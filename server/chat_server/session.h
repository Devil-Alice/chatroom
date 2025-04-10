#pragma once
#include "global.h"
#include "server.h"

#define BUFFER_MAX_LEN 8192

class Server;

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
    Package(const Package &package);
    ~Package();

    int16_t get_request_id() { return request_id_; }
    void set_request_id(int16_t request_id) { request_id_ = request_id; }

    int16_t get_message_length() { return message_length_; }

    std::string get_message() { return message_; }
    void set_message(std::string message)
    {
        message_ = message;
        message_length_ = message_.length();
    }

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

    // 从属的服务器
    std::shared_ptr<Server> server_;

    // session的uuid
    string uuid_;

    size_t head_length_;
    // 接收的请求数据包，不用队列的原因：接收时是通过链式回调接收的，只有在接收到一个数据包才会进行下一次的循环，所以request_package_在接收时是安全的，在接收到后，会将副本存入packages_request_
    std::shared_ptr<Package> request_;

    // 数据包队列，只用来存取，不用来读写,可以保证异步处理时的序列，同时防止数据覆盖，操作时加锁
    std::queue<std::shared_ptr<Package>> packages_request_;
    std::queue<std::shared_ptr<Package>> packages_response_;

    std::mutex mutex_request_;
    std::mutex mutex_response_;

    // 为了尽快接收客户端的请求，我们在读取完一个数据包后，剩下的工作交给处理线程，然后继续开始接收请求
    std::atomic<bool> flag_stop_;
    std::condition_variable cond_handle_request_;
    std::condition_variable cond_send_response_;
    std::thread thread_send_response_;
    void init_thread_send_response();

public:
    using read_handler = std::function<void(boost::system::error_code, size_t)>;
    Session(asio::io_context &ioc, std::shared_ptr<Server> server);
    ~Session();
    tcp::socket &get_socket();
    string get_uuid();
    // 添加请求，将请求放入packages_request_中
    void add_request(std::shared_ptr<Package> package);
    // 获取请求，从packages_request_中移除
    std::shared_ptr<Package> get_request();
    // 设置回复，将回复放入packages_response_
    void add_response(std::shared_ptr<Package> package);
    // 添加响应，同时从packages_response_移除
    std::shared_ptr<Package> get_response();
    /// @brief 异步读取固定长度的数据
    /// @param handler 读取完毕，或者出错的处理函数
    /// @note length_read不应该手动传入，而是作为反复调用该函数所需的参数，自动传入
    /// @note 例如：需要读10个字节，但是第一次只读了2个字节，此时在内部会自动将length_read = 2传入此函数，继续读取，直到读完10个字节
    void async_read_fixed_length(size_t length, read_handler handler, size_t length_read = 0);
    // 异步读取整个数据包
    void receive_package();
    // 处理请求
    void handle_request();
    // 向客户端发送数据包
    void send_package();
    // 异步读取头部
    void read_head();
    // 异步读取消息的函数
    void read_message();
};
