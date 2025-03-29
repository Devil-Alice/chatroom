#pragma once
#include "global.h"
#include "session.h"

class TcpService : public Singleton<TcpService>
{
    friend class Singleton<TcpService>;
    using string = std::string;
private:
    using TcpHandler = std::function<std::shared_ptr<Package>(std::shared_ptr<Package>)>;
    std::map<REQUEST_ID, TcpHandler> services_;
    TcpService();
public:
    ~TcpService();
    void register_service(REQUEST_ID request_id, TcpHandler handler);
    std::shared_ptr<Package> handle_request(std::shared_ptr<Package> package);
    
};