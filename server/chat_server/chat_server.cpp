#include "global.h"
#include "io_context_pool.h"
#include "server.h"

int main()
{
    try
    {
        auto &pool = IoContextPool::instance();

        auto ioc = asio::io_context{1};
        asio::signal_set signal(ioc, SIGINT, SIGTERM);
        signal.async_wait([&ioc, &pool](boost::system::error_code err_code, int signal){
            if (err_code)
            {
                std::cout << "signal error: " << err_code.message() << std::endl;
                return;
            }

            ioc.stop();
            pool.stop();
            return;
        });

        // 解析配置文件
        int port = 55555;

        std::shared_ptr<Server> server = std::make_shared<Server>(port, ioc);
        server->accept_client();
        std::cout << "chat server running..." << std::endl;
        ioc.run();
        
    }
   catch (std::exception &ex)
    {
        std::cout << "main exception occured: " << ex.what() << std::endl;
    }

    return 0;
}