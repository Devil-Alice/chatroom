#pragma once
#include <string>
#include <vector>
#include "json_object.h"

// 限定容器内部职能装JsonObject类型的智能指针，同时实现多态，并且自身支持序列化/反序列化
template <typename T>
class MyVector : public std::vector<std::shared_ptr<JsonObject>>, public JsonObject
{
    using string = std::string;

public:
    ~MyVector() override
    {
        // 释放资源
        this->clear();
    }
    Json::Value to_json() override
    {
        {
            Json::Value json_array(Json::arrayValue);
            for (std::shared_ptr<JsonObject> &item : *this)
            {
                Json::Value json_obj = item->to_json();
                json_array.append(json_obj);
            }
            return json_array;
        }
    }
    void from_json_string(string json_string) override
    {
        Json::Value json_array = parse_json_string(json_string);
    
        this->clear();
        for (auto &item : json_array)
        {
            std::shared_ptr<JsonObject> obj = std::make_shared<T>();
            obj->from_json_string(item.toStyledString());
            this->push_back(obj);
        }
    }
};