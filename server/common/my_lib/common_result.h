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
    std::shared_ptr<JsonObject> data_;

public:
    CommonResult() {};
    CommonResult(int status, string message, std::shared_ptr<JsonObject> data = nullptr)
        : status_(status), message_(message), data_(data) {}
    ~CommonResult() {};

    // 方便链式调用
    CommonResult &set(int status, string message, std::shared_ptr<JsonObject> data = nullptr)
    {
        status_ = status;
        message_ = message;
        data_ = data;
        return *this;
    }

    string to_json_string()
    {
        Json::Value root;

        root["status"] = status_;
        root["message"] = message_;
        if (data_)
            root["data"] = data_->to_json();
        else
            root["data"] = Json::Value();

        return root.toStyledString();
    }
};
