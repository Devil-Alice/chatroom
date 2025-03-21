#include "my_stl.h"
#include <jsoncpp/json/json.h>

template<typename T>
std::string MyVector<T>::to_json_string()
{
    Json::Value json_array(Json::arrayValue);
    Json::Reader reader;
    for (T &item : *this)
    {
        string item_str = item.to_json_string();
        Json::Value root;
        reader.parse(item_str, root);
        json_array.append(root);
    }
    return json_array.toStyledString();
}

template<typename T>
void MyVector<T>::from_json_string(std::string json_string) 
{
    Json::Reader reader;
    Json::Value json_array(Json::arrayValue);

    bool success = reader.parse(json_string, json_array);
    if (!success || !json_array.isArray())
        throw std::runtime_error("MyVector from_json_string error");

    this->clear();
    for (auto &item : json_array)
    {
        T obj;
        obj.from_json_string(item.toStyledString());
        this->push_back(obj);
    }
}