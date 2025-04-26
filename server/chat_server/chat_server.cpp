#include "global.h"
#include "io_context_pool.h"
#include "server.h"
#include "grpc_chat_server.h"

int main()
{
    try
    {
        // 创建ioc
        auto ioc = asio::io_context{1};
        asio::signal_set signal(ioc, SIGINT, SIGTERM);

        // 解析配置文件,创建chatserver的主server程序
        std::string port_str = app_config["chat_server_serlf"]["port"];
        std::string name = app_config["chat_server_serlf"]["name"];
        int port = std::atoi(port_str.data());
        std::shared_ptr<Server> server = std::make_shared<Server>(name, port, ioc);
        server->accept_client();

        // 创建grpc chat server
        grpc::ServerBuilder server_builder;
        std::string bind_ip = app_config["chat_server_serlf"]["bind_ip"];
        std::string grpc_chat_server_port = app_config["chat_server_serlf"]["grpc_chat_server_port"];
        server_builder.AddListeningPort(bind_ip + ":" + grpc_chat_server_port, grpc::InsecureServerCredentials());
        server_builder.RegisterService(&GrpcChatServer::instance());
        std::unique_ptr<grpc::Server> grpc_server(server_builder.BuildAndStart());

        signal.async_wait([&](boost::system::error_code err_code, int signal)
                          {
            if (err_code)
            {
                std::cout << "signal error: " << err_code.message() << std::endl;
                return;
            }

            grpc_server->Shutdown();
            ioc.stop();
            return; });

        std::cout << "chat server running..." << std::endl;

        // 在线程中启动grpc server
        std::thread(
            [server = grpc_server.get()]()
            {
                server->Wait();
            }
        ).detach();

        ioc.run();
    }
    catch (std::exception &ex)
    {
        std::cout << "main exception occured: " << ex.what() << std::endl;
    }

    return 0;
}