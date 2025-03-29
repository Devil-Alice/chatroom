#pragma once
#include <string>
#include <memory>
#include <jsoncpp/json/json.h>
#include "json_object.h"

// 网络请求的通用返回类型
class CommonResult
{
    using string = std::string;

private:
    // 结果状态
    int status_;
    string message_;
    // 响应的数据
    string data_;

public:
    // 方便链式调用
    CommonResult &set(int status, string message, string data = "")
    {
        status_ = status;
        message_ = message;
        data_ = data;
        return *this;
    }

    CommonResult() {};
    CommonResult(int status, string message, string data = "")
    {
        set(status, message, data);
    }
    ~CommonResult() {};

    string to_json_string()
    {
        Json::Value root;

        root["status"] = status_;
        root["message"] = message_;
        root["data"] = data_;

        return root.toStyledString();
    }
};
