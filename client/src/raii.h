#pragma once
#include <functional>

/// @brief RAII(Resource Acquisition Is Initialization)，一个简单的资源释放类
class Raii
{
    using RaiiHandler = std::function<void()>;

private:
    RaiiHandler raii_handler_;

public:
    Raii(RaiiHandler raii_handler) : raii_handler_(raii_handler) {}
    ~Raii()
    {
        if (raii_handler_)
        {
            raii_handler_();
        }
    }
};