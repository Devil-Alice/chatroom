#include "server.h"

Server::Server(asio::io_context &ioc, unsigned int port)
    : ioc_(ioc), acceptor_(ioc, tcp::endpoint(tcp::v4(), port)), socket_(ioc)
{
}

void Server::start_listen_client()
{
    auto self = shared_from_this();
    acceptor_.async_accept(socket_, [self](beast::error_code error_code)
                           {
            try{
                //如果出现异常，放弃此次的socket，并且继续监听
                if (error_code)
                {
                    self->start_listen_client();
                    return;
                }

                //接收到了一个新链接，创建HttpConnection类，将这个socket传入
                //此处用move是因为socket没有拷贝构造函数
                std::make_shared<HttpConnection>(std::move(self->socket_))->start_listen_events();
                self->start_listen_client();

            } catch(std::exception &ex)
            {
                
            } });
}
