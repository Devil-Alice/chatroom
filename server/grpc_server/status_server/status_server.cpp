#include <thread>
#include <memory>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <grpcpp/grpcpp.h>
#include "grpc_status_server.h"

namespace beast = boost::beast;
namespace asio = boost::asio;
using boost::asio::ip::tcp;

int main()
{
    // 从配置文件中读取服务器信息
    std::string bind_ip = app_config["status_server"]["bind_ip"];
    std::string port = app_config["status_server"]["port"];

    // 创建状态服务
    std::string server_address = bind_ip + ":" + port;
    GrpcStatusServer status_server;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&status_server);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

    // 用boost库接收终止信号，退出进程
    asio::io_context ioc{1};
    asio::signal_set signal(ioc, SIGINT, SIGTERM);

    signal.async_wait([&server](boost::system::error_code err_code, int signal) -> void
                      {

        if (err_code)
        {
            std::cout << "signal.async_wait error: " << err_code.message() << std::endl;
            return;
        }

        // 终止server
        server->Shutdown();
        std::cout << "signal.async_wait triggered: ioc exit"<< std::endl;
        return; });

    // 由于ioc会阻塞，所以要单独运行一个线程
    std::thread([&ioc]()
                { ioc.run(); })
        .detach();

    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait(); // Block until the server shuts down.
    ioc.stop();
}