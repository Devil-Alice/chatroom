#pragma once
#include "global.h"

template <typename T>
class Singleton
{
protected:
    Singleton(){};

public:
    ~Singleton(){};
    // static T &instance()
    // {
    //     static T instance_;
    //     return instance_;
    // }
    static std::shared_ptr<T> instance()
    {
        static std::shared_ptr<T> instance_ = std::shared_ptr<T>(new T);
        return instance_;
    }
};