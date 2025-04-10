#include "session.h"
#include "tcp_service.h"

Package::Package()
{
    request_id_ = REQUEST_ID::UNKNOWN;
    message_length_ = 0;
    buffer_ = new char[BUFFER_MAX_LEN + 1]{0};
}

Package::Package(const Package &package)
{
    request_id_ = package.request_id_;
    message_length_ = package.message_length_;
    message_ = package.message_;
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

    request_id_ = ntohs(request_id_);
    message_length_ = ntohs(message_length_);

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

char *Package::build_buffer()
{
    // 转换字节序
    request_id_ = htons(request_id_);
    message_length_ = htons(message_length_);

    // 将包头写入buffer
    memcpy(buffer_, &request_id_, sizeof(request_id_));
    memcpy(buffer_ + sizeof(request_id_), &message_length_, sizeof(message_length_));
    // 将消息体写入buffer
    memcpy(buffer_ + sizeof(request_id_) + sizeof(message_length_), message_.data(), message_.length());

    return buffer_;
}

size_t Package::get_total_length()
{
    return 2 + 2 + message_.length();
}

void Session::init_thread_send_response()
{
    thread_send_response_ = std::thread([this](){
        // 持续发送数据包
        while (!flag_stop_)
        {
            // todo :使用条件变量优化获取请求的函数，在没有请求时阻塞，避免占用资源过多

            // 先处理数据包
            handle_request();
    
            // 异步发送response
            send_package();
        }
    });
}

Session::Session(asio::io_context &ioc, std::shared_ptr<Server> server) 
: ioc_(ioc), socket_(ioc), server_(server)
{
    uuid_ = my_utils::generate_uuid();
    head_length_ = sizeof(uint16_t) * 2;
    request_ = std::make_shared<Package>();
    flag_stop_ = false;
    init_thread_send_response();
}

Session::~Session()
{
    flag_stop_ = true;
    cond_handle_request_.notify_all();
    cond_send_response_.notify_all();
    thread_send_response_.join();

    mutex_request_.lock();
    while (!packages_request_.empty())
    {
        packages_request_.pop();
    }
    mutex_request_.unlock();

    mutex_response_.lock();
    while (!packages_response_.empty())
    {
        packages_response_.pop();
    }
    mutex_response_.unlock();
}

tcp::socket &Session::get_socket()
{
    return socket_;
}

std::string Session::get_uuid()
{
    return uuid_;
}

void Session::add_request(std::shared_ptr<Package> package)
{
    std::lock_guard<std::mutex> locker(mutex_request_);
    // 这里要emplace调用拷贝构造，创建一个package的副本
    packages_request_.emplace(std::make_shared<Package>(*package));
    cond_handle_request_.notify_one();
    return;
}

std::shared_ptr<Package> Session::get_request()
{
    std::unique_lock<std::mutex> locker(mutex_request_);
    cond_handle_request_.wait(locker, 
    [this]() -> bool
    {
        if (flag_stop_)
        {
            return true;
        }
        else if (packages_request_.size() <= 0)
        {
            return false;
        }

        return true;
    });

    if (flag_stop_)
        return nullptr;
    
    auto pkg = packages_request_.front();
    packages_request_.pop();
    return pkg;
}

void Session::add_response(std::shared_ptr<Package> package)
{
    std::lock_guard<std::mutex> locker(mutex_response_);
    packages_response_.push(package);
    return;
}

std::shared_ptr<Package> Session::get_response()
{
    std::lock_guard<std::mutex> locker(mutex_response_);
    if (packages_response_.size() <= 0)
        return nullptr;
    auto pkg = packages_response_.front();
    packages_response_.pop();
    return pkg;
}

void Session::async_read_fixed_length(size_t length, read_handler handler, size_t length_read)
{
    auto self = shared_from_this();
    char *buf = request_->buffer_;
    // async_read_some接收char*类型的缓冲区时，需要手动指定写的位置，否则会覆盖原有数据
    // 此处应该写入的位置为 起始地址 + 已经读取的长度，能写入的大小为，期望读取的长度 - 已经读取的长度
    socket_.async_read_some(asio::buffer(buf + length_read, length - length_read), 
    [self, handler, length, length_read](boost::system::error_code err_code, size_t size)
    {
        // 计算当前长度 = 本次读取的长度size + 之前累积读取的长度length_read
        size_t cur_len = size + length_read;
        // 出错了，或者是长度达到，调用回调
        if (err_code || cur_len >= length)
        {
            handler(err_code, cur_len);
            return;  
        }

        // 没有达到预期长度，继续接收
        self->async_read_fixed_length(length - cur_len, handler, cur_len);
        return; 
    });
}

void Session::receive_package()
{
    // 读取头部，并且头部读取完了之后执行读取消息的动作
    read_head();
}

void Session::handle_request()
{
    auto pkg = get_request();
    if (pkg == nullptr)
        return;

    // handle_request一般不返回null，如果出错会以package返回一个错误包
    auto response = TcpService::instance().handle_request(pkg);
    
    add_response(response);
    return;
}

void Session::send_package()
{
    auto pkg = get_response();
    if(pkg == nullptr)
        return;
    pkg->build_buffer();
    socket_.async_send(asio::buffer(pkg->buffer_, pkg->get_total_length()), 
    [pkg](boost::system::error_code err_code, size_t size)
    {
        // 如果出错，可能是网络错误、客户端掉线，看情况决定是否需要重发
        // 如果需要重发，将数据包重新放回
        if (err_code || size < pkg->get_total_length())
        {
            std::cout << "send_package error: " << err_code.message() << std::endl; 
            return;
        }
    });
}

void Session::read_head()
{
    auto self = shared_from_this();
    // 读取固定长度的头
    async_read_fixed_length(head_length_, [self](boost::system::error_code err_code, size_t size)
                            {
        if (err_code || size < self->head_length_)
        {
            std::cout << "read_head error: " << err_code.message() << std::endl; 
            return;
        }
        // 头部读取完毕，开始解析          
        
        self->request_->parse_head();

        // 头部解析完毕，开始读取消息
        self->read_message(); });
}

void Session::read_message()
{
    auto self = shared_from_this();
    async_read_fixed_length(request_->message_length_, [self](boost::system::error_code err_code, size_t size)
                            {

        if (err_code || size < (size_t)self->request_->message_length_)
        {
            std::cout << "read_message error: " << err_code.message() << std::endl;
            return;
        }            
        
        // 将接收到的消息转为string
        self->request_->parse_message();

        // 将解析好的消息放入request队列中，处理线程在内部会处理这个请求
        self->add_request(self->request_);

        // 调试信息
        std::cout << "read_message received a message: " << std::endl <<
        "reqest id: " << self->request_->get_request_id() << std:: endl <<
        "request message: " << self->request_->get_message() << std::endl;

        // 继续接收数据包
        self->receive_package();

        return; });
}
