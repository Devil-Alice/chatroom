#include <iostream>
#include <grpcpp/grpcpp.h>
#include "global.h"
#include "grpc_verify_server.h"


using namespace std;
using namespace grpc;

void run_server()
{
    string bind_ip = app_config["verify_server"]["bind_ip"];
    string port = app_config["verify_server"]["port"];
    
    string server_address = bind_ip + ":" + port;
    GrpcVerifyServer verify_service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, InsecureServerCredentials());
    builder.RegisterService(&verify_service);
    unique_ptr<Server> server(builder.BuildAndStart());
    cout << "Server listening on " << server_address << endl;
    server->Wait();
    
}

int main()
{
    run_server();
    return 0;
}