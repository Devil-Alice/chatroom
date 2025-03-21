#pragma once
#include <string>
#include <vector>
#include "json_object.h"

template <typename T>
class MyVector : public std::vector<T>, public JsonObject
{
    using string = std::string;

public:
    string to_json_string() override;
    void from_json_string(string json_string) override;
};
