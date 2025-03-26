#pragma once
#include "global.h"

#define BUFFER_MAX_LEN 8192

class Package
{
    // 请求数据包分为三部分：request_id(2字节)，message_length(2字节)，message
    friend class Session;

private:
    int16_t request_id_;
    int16_t message_length_;
    std::string message_;
    char *buffer_;

public:
    Package();
    ~Package();

    void clear_buffer();
    void parse_head();
    void parse_message();
    char *build_buffer();
};

// 会话类，表示与一个客户端连接通信的类
class Session : public std::enable_shared_from_this<Session>
{
    using string = std::string;

private:
    // 通信中交换数据的事件循环ioc
    asio::io_context &ioc_;
    // 与客户端连接的socket
    tcp::socket socket_;
    size_t head_length_;
    // 数据包队列，保证异步处理时的序列，同时防止数据覆盖
    std::queue<std::shared_ptr<Package>> packages_;

    std::mutex mutex_recv;
    std::mutex mutex_send;

public:
    using read_handler = std::function<void(boost::system::error_code, size_t)>;
    Session(asio::io_context &ioc);
    ~Session();
    /// @brief 异步读取固定长度的数据
    /// @param handler 读取完毕，或者出错的处理函数
    /// @note length_read不应该手动传入，而是作为反复调用该函数所需的参数，自动传入
    /// @note 例如：需要读10个字节，但是第一次只读了2个字节，此时在内部会自动将length_read = 2传入此函数，继续读取，直到读完10个字节
    void async_read_fixed_length(size_t length, read_handler handler, size_t length_read = 0);
    // 异步读取整个数据包
    void receive_package();
    // 向客户端发送数据包
    void send_package();
    // 异步读取头部
    void read_head();
    // 异步读取消息的函数
    void read_message();
};
