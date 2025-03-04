#pragma once
#include "singleton.h"
#include "json/json.h"

class HttpManager : public Singleton<HttpManager>
{
private:
    /* data */
public:
    HttpManager(/* args */);
    ~HttpManager();
};

HttpManager::HttpManager(/* args */)
{
}

HttpManager::~HttpManager()
{
}