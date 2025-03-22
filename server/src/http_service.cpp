#include "http_service.h"
#include "http_connection.h"
#include "verify_grpc_client.h"
#include "redis_manager.h"
#include "user_dao.h"
#include "user_service.h"


HttpService::HttpService()
{
    using namespace std;

    register_get("/verify_code", [](std::shared_ptr<HttpConnection> http_conn)
                {
        auto response_body_ostream = beast::ostream(http_conn->response_.body());
        try
        {
            // 获取手机号
            string phone = http_conn->get_params_["phone"];

            // 检查手机号是否传入
            if (phone.empty())
            {
                // 没有手机号
                cout << "error: phone not found" << endl;
                return false;
            }
            string verify_code = UserService::instance().generate_verify_code(phone);
            // 写入请求体
            http_conn->response_.set(http::field::content_type, "text/plain");
            response_body_ostream << verify_code;

            return true;
        }
        catch (std::exception &ex)
        {
            cout << "handle /verify_code failed: " << ex.what() << endl;
        }

        return true;
    });

    register_post("/verify_code", [](std::shared_ptr<HttpConnection> http_conn)
    {
        auto response_body_ostream = beast::ostream(http_conn->response_.body());
        CommonResult result(1, "获取验证码失败");
        try
        {
            // 获取手机号
            Json::Value root = JsonObject::parse_json_string(beast::buffers_to_string(http_conn->request_.body().data()));
            
            string phone = root["phone"].asString();

            // 检查手机号是否传入
            if (phone.empty())
            {
                // 没有手机号
                cout << "error: phone not found" << endl;
                return false;
            }
            string verify_code = UserService::instance().generate_verify_code(phone);
            // 写入请求体
            http_conn->response_.set(http::field::content_type, "text/plain");

            response_body_ostream << result.set(0, "ok").to_json_string();

            return true;
        }
        catch (std::exception &ex)
        {
            cout << "handle /verify_code failed: " << ex.what() << endl;
            response_body_ostream << result.set(1, "发送失败").to_json_string();
        }

        return true;
    });
   

    register_post("/user", [](std::shared_ptr<HttpConnection> http_conn)
                  {
        auto response_body_ostream = beast::ostream(http_conn->response_.body());
        CommonResult result(1, "注册用户失败");
        try 
        {
            string body_str = boost::beast::buffers_to_string(http_conn->request_.body().data());
            // 设置返回类型
            http_conn->response_.set(http::field::content_type, "text/plain");
    
            // 检查是否为空
            if (body_str.empty())
            {
                response_body_ostream << result.set(1, "信息请填写完整").to_json_string();
                return true;
            }
    
            // 解析json字符串为user
            Json::Reader reader;
            Json::Value root = JsonObject::parse_json_string(body_str);;
    
            string phone = root["phone"].asString();
            string verify_code = root["verify_code"].asString();
            string verify_code_redis = UserService::instance().get_verify_code_from_redis(phone);
            if (verify_code_redis.empty() || verify_code_redis != verify_code)
            {
                response_body_ostream << result.set(1, "验证码错误").to_json_string();
                return true;
            }
     
            string name = root["name"].asString();
            string password = root["password"].asString();
            User user(name, phone, password);
            bool success = UserService::instance().register_user(user);
            if (!success)
            {
                response_body_ostream << result.set(1, "注册失败").to_json_string();
                return true;
            }
    
            response_body_ostream << result.set(0, "注册成功").to_json_string();
            return true; 
        } 
        catch (std::exception &ex)
        {
            response_body_ostream << result.set(1, ex.what()).to_json_string();
            cout << "handle /user failed: " << ex.what() << endl;
        }
        return true;
    });

    register_post("/post_test", [](std::shared_ptr<HttpConnection> http_conn)
                  {
        string body_str = boost::beast::buffers_to_string(http_conn->request_.body().data());
        ;
        cout << "received body: " << body_str << endl;
        http_conn->response_.set(http::field::content_type, "text/json");
        Json::Value root;
        Json::Reader reader;
        Json::Value src_root;
        bool success = reader.parse(body_str, src_root, false);

        // 定义检测的key字符串
        std::string deteced_key = "phone";

        // 解析失败，或者不包含key值
        if (!success || !src_root.isMember(deteced_key))
        {
            cout << "failed to parse json data" << endl;
            // 这里返回的是true，因为json格式不对只是小问题，false是比较严重的情况
            root["error"] = (int)MY_STATUS_CODE::JSON_ERROR;
            // 格式化
            string styled_str = root.toStyledString();
            // 写入response
            beast::ostream(http_conn->response_.body()) << styled_str << endl;
            return true;
        }

        // 解析成功
        string value = src_root[deteced_key].asString();
        // 调用rpc方法获取验证码
        GetVerifyResponse rsp = VerifyGrpcClient::instance().get_rerify_code(value);
        if (rsp.error() != 0)
        {
        }
        cout << deteced_key << ": " << value << endl;

        // 构建成功的返回数据
        root["error"] = 0;
        root[deteced_key] = value;
        root["code"] = rsp.code();
        string styled_str = root.toStyledString();
        // 写入response
        beast::ostream(http_conn->response_.body()) << styled_str << endl;
        return true;
                  });
}

HttpService::~HttpService()
{
}

void HttpService::register_get(std::string url, HttpHandler handler)
{
    get_services_.insert(std::make_pair(url, handler));
}

void HttpService::register_post(std::string url, HttpHandler handler)
{
    post_services_.insert(std::make_pair(url, handler));
}

bool HttpService::handle_get(std::string url, std::shared_ptr<HttpConnection> http_conn)
{
    if (get_services_.find(url) == get_services_.end())
    {
        std::cout << "get reqest(" << url << ") not found" << std::endl;
        return false;
    }

    return get_services_[url](http_conn);
}

bool HttpService::handle_post(std::string url, std::shared_ptr<HttpConnection> http_conn)
{
    if (post_services_.find(url) == post_services_.end())
    {
        std::cout << "post reqest(" << url << ") not found" << std::endl;
        return false;
    }

    return post_services_[url](http_conn);
}
