#include "grpc_status_server.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

GrpcStatusServer::GrpcStatusServer()
{
    poll_idx_ = 0;

    ChatServer chat_server;

    chat_server.host = "127.0.0.1";
    chat_server.port = 8081;
    chat_servers_.push_back(chat_server);

    chat_server.port = 8082;
    chat_servers_.push_back(chat_server);
}

GrpcStatusServer::~GrpcStatusServer()
{

}

grpc::Status GrpcStatusServer::get_chat_server(grpc::ServerContext *context, const GetChatServerRequest *request, GetChatServerResponse *response)
{
    std::cout << "get_chat_server uid:" << request->uid() << std::endl;
    ChatServer server = chat_servers_[poll_idx_];
    poll_idx_ = (poll_idx_ + 1) % chat_servers_.size();

    boost::uuids::random_generator gen;
    std::string uuid = boost::uuids::to_string(gen());
    
    response->set_error(0);
    response->set_host(server.host);
    response->set_port(std::to_string(server.port));
    response->set_token(uuid);
    
    std::cout << "get_chat_server finished" << std::endl;
    return grpc::Status::OK;
}