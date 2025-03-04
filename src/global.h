#pragma once
#include <iostream>
#include <memory>
#include <mutex>
#include <chrono>
#include <functional>
#include <map>
#include "boost/beast/http.hpp"
#include "boost/beast.hpp"
#include "boost/asio.hpp"
#include "json/json.h"
#include "json/value.h"
#include "json/reader.h"
#include "singleton.h"

// 简化命名空间
namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
using tcp = boost::asio::ip::tcp;

enum class MY_ERROR
{
    JSON = 0x0001,
    TIMEOUT = 0x0002,
    NETWORK = 0x0003
};