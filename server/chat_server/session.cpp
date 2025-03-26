#include "session.h"

Package::Package()
{
    request_id_ = REQUEST_ID::UNKNOWN;
    message_length_ = 0;
    buffer_ = new char[BUFFER_MAX_LEN + 1]{0};
}

Package::~Package()
{
    if (buffer_)
    {
        delete[] buffer_;
        buffer_ = nullptr;
    }
}

void Package::clear_buffer()
{
    memset(buffer_, 0, BUFFER_MAX_LEN);
    return;
}

void Package::parse_head()
{
    memcpy(&request_id_, buffer_, sizeof(request_id_));
    memcpy(&message_length_, buffer_ + sizeof(request_id_), sizeof(message_length_));
    // 拷贝之后清空buff，为接收消息准备
    clear_buffer();
    return;
}

void Package::parse_message()
{
    // memcpy(message_, buffer_, message_length_);
    message_ = std::string(buffer_, message_length_);
    clear_buffer();
    return;
}





Session::Session(asio::io_context &ioc) : ioc_(ioc), socket_(ioc)
{
    head_length_ = sizeof(uint16_t) * 2;
}

Session::~Session()
{
    while(!packages_.empty())
    {
        packages_.pop();
    }
}

void Session::async_read_fixed_length(size_t length, read_handler handler, size_t length_read)
{
    auto self = shared_from_this();
    char *buf = packages_.back()->buffer_;
    // async_read_some接收char*类型的缓冲区时，需要手动指定写的位置，否则会覆盖原有数据
    // 此处应该写入的位置为 起始地址 + 已经读取的长度，能写入的大小为，期望读取的长度 - 已经读取的长度
    socket_.async_read_some(asio::buffer(buf + length_read, length - length_read), [self, handler, length, length_read](boost::system::error_code err_code, size_t size)
                            {
        if (size<0)
            std::cout << "async_read_some read " << size << " bytes" << std::endl;

        // 计算当前长度
        size_t cur_len = size + length_read;
        // 出错了，或者是长度达到，调用回调
        if (err_code || cur_len >= length)
        {
            handler(err_code, size);
            return;  
        }

        // 没有达到预期长度，继续接收
        self->async_read_fixed_length(length - cur_len, handler, cur_len);
        return; });
}

void Session::receive_package()
{
    // 构造一个空的额package
    packages_.emplace();
    auto self = shared_from_this();
    // 读取头部，并且头部读取完了之后执行读取消息的动作
    read_head();
}

void Session::send_package()
{
    auto self = shared_from_this();
    if (packages_.empty())
    {
        throw std::runtime_error("send_package error: no package to send");
        return;
    }

    socket_.async_send(asio::buffer(packages_.front()->message_), [self](boost::system::error_code err_code, size_t size)
    { 
        // 发送完毕，移除最前面的包
        self->packages_.pop();
        return; 
    });
}

void Session::read_head()
{
    auto self = shared_from_this();
    // 读取固定长度的头
    async_read_fixed_length(head_length_, [self](boost::system::error_code err_code, size_t size)
    {
        if (err_code || size <= self->head_length_)
        {
            std::cout << "read_head error: " << err_code.message() << std::endl; 
            return;
        }
        // 头部读取完毕，开始解析          
        auto pkg = self->packages_.back();
        pkg->parse_head();

        // 头部解析完毕，开始读取消息
        self->read_message(); });
}

void Session::read_message()
{
    auto self = shared_from_this();
    auto pkg = self->packages_.back();
    async_read_fixed_length(pkg->message_length_, [self, pkg](boost::system::error_code err_code, size_t size)
    {

        if (err_code || size <= (size_t)pkg->message_length_)
        {
            std::cout << "read_message error: " << err_code.message() << std::endl;
            return;
        }            
        
        // 将接收到的消息转为string
        pkg->parse_message();

        // 调用service处理这个package的请求

        // 判断处理的返回值

        // 处理完毕，发送回复
        self->send_package();

        // 回复完毕，继续接收数据包
        self->receive_package();

        return; });
}
