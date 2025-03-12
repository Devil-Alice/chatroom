#include "server.h"
#include "io_context_pool.h"

Server::Server(asio::io_context &ioc, unsigned int port)
    : ioc_(ioc), acceptor_(ioc, tcp::endpoint(tcp::v4(), port))
{
}

void Server::start_listen_client()
{
    auto self = shared_from_this();
    asio::io_context &ioc = IoContextPool::instance().get_io_context();
    //接收到了一个新链接，创建HttpConnection类
    std::shared_ptr<HttpConnection> http_conn = std::make_shared<HttpConnection>(ioc);
    auto &sock = http_conn->get_socket();
    acceptor_.async_accept(sock, [self, http_conn](beast::error_code error_code)
                           {
            try{
                //如果出现异常，放弃此次的socket，并且继续监听
                if (error_code)
                {
                    self->start_listen_client();
                    return;
                }

                
                //此处用move是因为socket没有拷贝构造函数
                http_conn->start_listen_events();
                self->start_listen_client();

            } catch(std::exception &ex)
            {
                
            } });
}
