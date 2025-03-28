#include "tcp_service.h"

TcpService::TcpService()
{
    register_service(REQUEST_ID::CHAT_LOGIN, [](std::shared_ptr<Package> package){
        // 查询数据库，用户是否存在
        
        // 从状态服务获取该用户的token

        // 匹配token是否正确

        return nullptr;
    });
}

TcpService::~TcpService()
{
}

void TcpService::register_service(REQUEST_ID request_id, TcpHandler handler)
{
    services_.insert(std::make_pair(request_id, handler));
    return;
}

std::shared_ptr<Package> TcpService::handle_request(std::shared_ptr<Package> package)
{
    REQUEST_ID request_id = (REQUEST_ID)package->get_request_id();
    if (services_.find(request_id) == services_.end())
    {
        std::cout << "tcp reqest(" << request_id << ") not found" << std::endl;
        return nullptr;
    }

    return services_[request_id](package);
}
