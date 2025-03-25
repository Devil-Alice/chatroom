#pragma once
#include "global.h"
#include "http_connection.h"

class Server : public std::enable_shared_from_this<Server>
{
private:
    asio::io_context &ioc_;
    tcp::acceptor acceptor_;

public:
    Server(asio::io_context &ioc, unsigned int port);
    void start_listen_client();
};