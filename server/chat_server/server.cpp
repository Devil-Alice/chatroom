#include "server.h"

Server::Server(int port, asio::io_context &ioc)
    : port_(port), ioc_(ioc), acceptor_(ioc_, tcp::endpoint(tcp::v4(), port_))
{
}

Server::~Server()
{
    ioc_.stop();
}

void Server::accept_client()
{
    auto self = shared_from_this();

    asio::io_context &ioc = IoContextPool::instance().get_io_context();
    std::shared_ptr<Session> session = std::make_shared<Session>(ioc, self);
    tcp::socket &socket = session->get_socket();

    acceptor_.async_accept(socket,
    [self, session](boost::system::error_code err_code)
    {      
        if (err_code)
        {
            std::cout << "async_accept err: " << err_code.message() << std::endl;
            return;
        }

        // 成功接收到一个客户端连接
        self->sessions_.insert(std::make_pair(session->get_id(), session));
        
        // 让session启动
        session->start();

        // 继续接受链接
        self->accept_client();
    });

}

void Server::remove_session(string session_uuid)
{
    sessions_.erase(session_uuid);
    return;
}
