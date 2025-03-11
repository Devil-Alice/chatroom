#include "verify_grpc_server.h"
#include <random>

Status VerifyGrpcServer::get_verify_code(ServerContext *context, const GetVerifyRequest *req, GetVerifyResponse *rsp)
{

    //  获取手机号
    std::string phone = req->phone();

    // 使用 C++11 的随机数引擎
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(1000, 9999);
    int code = distrib(gen); // 生成随机四位数

    // 设置发送的响应
    rsp->set_error(0);
    rsp->set_code(std::to_string(code));
    rsp->set_phone(phone);

    std::cout << "receive get_verify_code request" << std::endl;
    std::cout << "phone: " << req->phone() << ", generate code: " << code << std::endl;

    return Status::OK;
}