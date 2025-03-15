#pragma once
#include "global.h"

class HttpConnection;
using HttpHandler = std::function<bool(std::shared_ptr<HttpConnection>)>;

class HttpService : public Singleton<HttpService>
{
    friend class Singleton<HttpService>;

private:
    HttpService();
    std::map<std::string, HttpHandler> get_services_;
    std::map<std::string, HttpHandler> post_services_;
    std::map<std::string, HttpHandler> delete_services_;
    std::map<std::string, HttpHandler> update_services_;

public:
    ~HttpService();
    void register_get(std::string url, HttpHandler handler);
    void register_post(std::string url, HttpHandler handler);
    bool handle_get(std::string url, std::shared_ptr<HttpConnection> http_conn);
    bool handle_post(std::string url, std::shared_ptr<HttpConnection> http_conn);

private:
};
