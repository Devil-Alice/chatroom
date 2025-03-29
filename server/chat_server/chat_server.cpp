#include "global.h"
#include "io_context_pool.h"

int main()
{
    try
    {
        auto &pool = IoContextPool::instance();

        auto ioc = asio::io_context{1};
        asio::signal_set signal(ioc, SIGINT, SIGTERM);
        signal.async_wait([&](boost::system::error_code err_code, int signal_num){
            if (err_code)
            {
                std::cout << "signal error: " << err_code.message() << std::endl;
                return;
            }

            pool.stop();
            ioc.stop();
            return;
        });

        ioc.run();
        std::cout << "chat server running..." << std::endl;
    }
   catch (std::exception &ex)
    {
        std::cout << "main exception occured: " << ex.what() << std::endl;
    }

    return 0;
}