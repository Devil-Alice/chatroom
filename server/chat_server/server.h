#pragma once
#include "global.h"
#include "session.h"
#include "io_context_pool.h"

class Session;

// 专门监听客户端连接的server，建立连接后交由session处理请求/回复
class Server : public std::enable_shared_from_this<Server>
{
    using string = std::string;
private:
    // 服务器信息，通过配置文件自动解析
    int port_;

    // asio框架中，通信所需的对象：
    asio::io_context &ioc_;
    tcp::acceptor acceptor_;

    // 保存的session, key是token
    std::map<string, std::shared_ptr<Session>> sessions_;
    
public:
    Server(int port, asio::io_context &ioc);
    ~Server();
    void accept_client();

};