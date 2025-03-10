#pragma once
#include "global.h"

template <typename T>
class Singleton
{
protected:
    Singleton(){};

public:
    ~Singleton(){};
    static T &instance()
    {
        static T instance_;
        return instance_;
    }
};