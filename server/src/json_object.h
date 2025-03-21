#pragma once
#include <string>
#include <jsoncpp/json/json.h>

class JsonObject
{
    using string = std::string;

public:
    virtual ~JsonObject(){};
    static Json::Value parse_json_string(string json_string)
    {
        Json::Reader reader;
        Json::Value root;

        bool success = reader.parse(json_string, root);
        if (!success || !root.isObject())
            throw std::runtime_error("JsonObject parse_json_string error");
        return root;
    }

    virtual Json::Value to_json() = 0;
    virtual string to_json_string() { return to_json().toStyledString(); }
    virtual void from_json_string(string json_string) = 0;
};