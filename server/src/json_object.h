#pragma once
#include <string>

class JsonObject
{
    using string = std::string;

public:
    virtual string to_json_string() = 0;
    virtual void from_json_string(string json_string) = 0;
};