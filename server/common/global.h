#pragma once
#include <iostream>
#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <functional>
#include <atomic>
#include <queue>
#include <map>
#include <vector>
// jsoncpp 头文件
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>
// boost http网络相关头文件
#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
// grpc 头文件
#include <grpcpp/grpcpp.h>
// redis hiredis头文件
#include <hiredis/hiredis.h>
// mysql c++头文件
#include <mysql/jdbc.h>

#include "my_grpc.pb.h"
#include "my_grpc.grpc.pb.h"
#include "singleton.h"
#include "json_object.h"
#include "raii.h"
#include "config_manager.h"
#include "my_utils.h"
#include "common_result.h"
#include "model.h"


// 简化命名空间
namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
// 简化类名tcp
using tcp = boost::asio::ip::tcp;


// 引入作用域
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;


using my_grpc::GetVerifyRequest;
using my_grpc::GetVerifyResponse;

using my_grpc::GetChatServerRequest;
using my_grpc::GetChatServerResponse;

using my_grpc::UserLoginRequest;
using my_grpc::UserLoginResponse;

const std::string verify_code_prefix = "verify_code_";

enum MY_STATUS_CODE
{
    SUCCESS = 0,
    ERROR = 1,
    JSON_ERROR = 1001,
    RPC_FAILED = 1002,
    TIMEOUT = 1003,
    VERIFY_CODE_EXPIRED = 1004,
    VERIFY_CODE_ERROR = 1005,
    USER_EXIST = 1006,
    PHONE_INVALID = 1007,
    PASSWORD_ERROR = 1008,
    NETWORK_FAILED = 1009,
    TOKEN_INVALID = 1010,
    USER_NOT_FOUND = 1011,
    DATABASE_FAILED = 1012,
};

enum REQUEST_ID
{
    UNKNOWN = 1000,
    GET_VERIFY_CODE = 1001, //获取验证码
    REGISTER_USER = 1002, //用户注册
    USER_LOGIN = 1003, // 用户登录
    CHAT_LOGIN = 1004,  //聊天服务器登录
    SEARCH_CONTENT = 1005, // 搜索内容
    SNED_FRIEND_APPLY = 1006, // 发送好友申请
    QUERY_FRIEND_APPLY = 1007, // 查询好友申请
    HANDLE_FRIEND_APPLY = 1008, // 处理好友申请
};