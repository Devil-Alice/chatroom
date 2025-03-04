#pragma once
#include "global.h"

// class HttpService;

class HttpConnection : public std::enable_shared_from_this<HttpConnection>
{
    friend class HttpService;

private:
    tcp::socket socket_;
    beast::flat_buffer buffer_{8192};
    http::request<http::dynamic_body> request_;
    http::response<http::dynamic_body> response_;
    asio::steady_timer timer_{socket_.get_executor(), std::chrono::seconds(60)};
    std::string request_url_;
    std::map<std::string, std::string> get_params_;

public:
    HttpConnection(tcp::socket socket);
    ~HttpConnection();
    void start_listen_events();

private:
    void check_timeout();
    void handle_request();
    void send_response();
    void parse_url(const std::string &url);
};


std::string url_encode(const std::string& str);
std::string url_decode(const std::string& encoded);