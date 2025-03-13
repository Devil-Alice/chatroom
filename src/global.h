#pragma once
#include <iostream>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <functional>
#include <atomic>
#include <queue>
#include <map>
#include <vector>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>
#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <grpcpp/grpcpp.h>
#include "singleton.h"
#include "config_manager.h"
#include "my_message.pb.h"
#include "my_message.grpc.pb.h"

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

enum MY_ERROR_CODE
{
    JSON = 0x0001,
    RPC_RAILED = 0x002,
    TIMEOUT = 0x0003,
    NETWORK = 0x0004
};