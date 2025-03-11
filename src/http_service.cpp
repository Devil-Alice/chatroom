#include "http_service.h"
#include "http_connection.h"
#include "verify_grpc_client.h"

HttpService::HttpService()
{
    using namespace std;
    regist_get("/get_test", [](std::shared_ptr<HttpConnection> http_conn)
    {
        beast::ostream(http_conn->response_.body()) << "recieve /get_test request" << std::endl;;
        for (auto & elem : http_conn->get_params_)
        {
            beast::ostream(http_conn->response_.body())<< elem.first << ": " << elem.second << std::endl;
    } });


    regist_post("/varify_code", [](std::shared_ptr<HttpConnection> http_conn){
        string body_str = boost::beast::buffers_to_string(http_conn->request_.body().data());;
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
            //这里返回的是true，因为json格式不对只是小问题，false是比较严重的情况
            root["error"] = (int)MY_ERROR_CODE::JSON;
            //格式化
            string styled_str = root.toStyledString();
            //写入response
            beast::ostream(http_conn->response_.body()) << styled_str << endl;
            return true;
        }

        // 解析成功
        string value = src_root[deteced_key].asString();
        //调用rpc方法获取验证码
        GetVerifyResponse rsp = VerifyGrpcClient::instance().get_rerify_code(value);
        if (rsp.error() != 0)
        {
            
        }
        cout << deteced_key << ": " << value << endl;


        //构建成功的返回数据
        root["error"] = 0;
        root[deteced_key] = value;
        root["code"] = rsp.code();
        string styled_str = root.toStyledString();
        //写入response
        beast::ostream(http_conn->response_.body()) << styled_str << endl;
        return true;
        
        

    });
}

HttpService::~HttpService()
{
}

void HttpService::regist_get(std::string url, HttpHandler handler)
{
    get_services_.insert(std::make_pair(url, handler));
}

void HttpService::regist_post(std::string url, HttpHandler handler)
{
    post_services_.insert(std::make_pair(url, handler));
}

bool HttpService::handle_get(std::string url, std::shared_ptr<HttpConnection> http_conn)
{
    if (get_services_.find(url) == get_services_.end())
    {
        // std::cout << "get reqest(" << url << ") not found" << std::endl;
        return false;
    }

    get_services_[url](http_conn);
    return true;
}

bool HttpService::handle_post(std::string url, std::shared_ptr<HttpConnection> http_conn)
{
    if (post_services_.find(url) == post_services_.end())
    {
        // std::cout << "get reqest(" << url << ") not found" << std::endl;
        return false;
    }

    post_services_[url](http_conn);
    return true;
}
