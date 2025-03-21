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
#include <mysql-cppconn/mysql/jdbc.h>

#include "my_message.pb.h"
#include "my_message.grpc.pb.h"
#include "singleton.h"
#include "json_object.h"
#include "common_result.h"
#include "raii.h"
#include "config_manager.h"
#include "my_utils.h"


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

using my_message::GetVerifyRequest;
using my_message::GetVerifyResponse;
using my_message::VerifyService;

const std::string verify_code_prefix = "verify_code_";

enum MY_STATUS_CODE
{
    SUCCESS = 0,
    JSON_ERROR = 1001,
    RPC_RAILED = 1002,
    TIMEOUT = 1003,
    VERIFY_CODE_EXPIRED = 1004,
    VERIFY_CODE_ERROR = 1005,
    USER_EXIST = 1006,
    PHONE_INVALID = 1007,
    PASSWORD_ERROR = 1008,
    NETWORK_FAILED = 1009
};

