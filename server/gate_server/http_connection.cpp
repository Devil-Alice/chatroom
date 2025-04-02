#include "http_connection.h"
#include "http_service.h"

HttpConnection::HttpConnection(asio::io_context &ioc) : socket_(ioc)
{
}

HttpConnection::~HttpConnection()
{
}

tcp::socket &HttpConnection::get_socket()
{
    return this->socket_;
}

void HttpConnection::start_listen_events()
{
    auto self = shared_from_this();

    http::async_read(socket_, buffer_, request_, [self](beast::error_code ec, std::size_t receive_size)
                     {
        try{
            if (ec)
            {
                std::cout << "http async read error" << ec.message() << std::endl;
                return;
            }

            //忽略已发送字节数
            boost::ignore_unused(receive_size);
            self->handle_request();
            self->check_timeout();

        } catch (std::exception &ex)
        {
            std::cout << "async_read exception: " << ex.what() << std::endl;
        } });
}

void HttpConnection::check_timeout()
{
    auto self = shared_from_this();

    timer_.async_wait([self](beast::error_code ec)
                      { self->socket_.shutdown(tcp::socket::shutdown_send, ec); });
}

void HttpConnection::handle_request()
{
    response_.version(request_.version());
    // 不需要长连接
    response_.keep_alive(false);
    // 处理get请求
    if (request_.method() == http::verb::get)
    {
        parse_url(request_.target());
        bool success = HttpService::instance().handle_get(request_url_, shared_from_this());
        if (!success)
        {
            response_.result(http::status::not_found);
            response_.set(http::field::content_type, "text/plain");
            beast::ostream(response_.body()) << "url not found\r\n";
            send_response();
            return;
        }

        response_.result(http::status::ok);
        response_.set(http::field::server, "GateServer");
        send_response();
    }

    // 处理post请求
    if (request_.method() == http::verb::post)
    {
        parse_url(request_.target());
        bool success = HttpService::instance().handle_post(request_url_, shared_from_this());
        if (!success)
        {
            response_.result(http::status::not_found);
            response_.set(http::field::content_type, "text/plain");
            beast::ostream(response_.body()) << "url not found\r\n";
            send_response();
            return;
        }

        response_.result(http::status::ok);
        response_.set(http::field::server, "GateServer");
        send_response();
    }
}

void HttpConnection::send_response()
{
    auto self = shared_from_this();
    response_.content_length(response_.body().size());

    http::async_write(socket_, response_, [self](beast::error_code ec, std::size_t send_size)
                      {
                          // 关闭这一方的连接
                          self->socket_.shutdown(tcp::socket::shutdown_send, ec);
                          self->timer_.cancel(); });
}

// 解析 URL
void HttpConnection::parse_url(const std::string &url)
{
    size_t pos = url.find('?');
    if (pos == std::string::npos)
    {
        // 没有参数，整个 URL 作为路径
        request_url_ = url;
        return;
    }

    // 获取 URL 路径
    request_url_ = url.substr(0, pos);

    // 解析参数部分
    std::string query_string = url.substr(pos + 1);
    std::istringstream query_stream(query_string);
    std::string key_value;

    while (std::getline(query_stream, key_value, '&'))
    {
        size_t equal_pos = key_value.find('=');
        if (equal_pos != std::string::npos)
        {
            std::string key = key_value.substr(0, equal_pos);
            std::string value = key_value.substr(equal_pos + 1);
            get_params_[url_decode(key)] = url_decode(value);
        }
    }
}

// URL 编码函数
std::string url_encode(const std::string &str)
{
    std::ostringstream encoded;
    for (unsigned char c : str)
    {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
        {
            encoded << c; // 直接添加 ASCII 可见字符
        }
        else if (c == ' ')
        {
            encoded << "%20"; // 空格转换为 %20
        }
        else
        {
            encoded << '%' << std::uppercase << std::hex << (int)c; // 转换非 ASCII
        }
    }
    return encoded.str();
}

// URL 解码函数
std::string url_decode(const std::string &encoded)
{
    std::ostringstream decoded;
    for (size_t i = 0; i < encoded.length(); i++)
    {
        if (encoded[i] == '%' && i + 2 < encoded.length())
        {
            std::string hex_str = encoded.substr(i + 1, 2);                         // 取出 % 后的两位十六进制字符
            char decoded_char = static_cast<char>(std::stoi(hex_str, nullptr, 16)); // 转换为字符
            decoded << decoded_char;
            i += 2; // 跳过已经解码的部分
        }
        else if (encoded[i] == '+')
        {
            decoded << ' '; // '+' 转为空格
        }
        else
        {
            decoded << encoded[i];
        }
    }
    return decoded.str();
}