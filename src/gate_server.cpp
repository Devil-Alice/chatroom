#include <iostream>
#include "global.h"
#include "server.h"

int main()
{
    try
    {
        std::cout << "server init..." << std::endl;

        std::string port_ = app_config["gate_server"]["port"];

        unsigned short port = std::atoi(port_.data());
        asio::io_context ioc{1};
        asio::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&ioc](beast::error_code ec, int signal)
                           {
            if (ec)
            {
                std::cout << "error_code: " << ec.message() << std::endl;
                return;
            }

            ioc.stop(); });

        std::make_shared<Server>(ioc, port)->start_listen_client();
        ioc.run();
    }
    catch (std::exception ex)
    {
        std::cout << "excetpion: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}