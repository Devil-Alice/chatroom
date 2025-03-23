#include <iostream>
#include <grpcpp/grpcpp.h>
#include "grpc_verify_server.h"


using namespace std;
using namespace grpc;

void run_server()
{
    string server_address = "0.0.0.0:50051";
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