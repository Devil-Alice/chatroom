#pragma once
#include "global.h"

class RedisConnectionPool
{
    using string = std::string;

private:
    size_t pool_size_;
    string host_;
    int port_;
    std::queue<redisContext *> connections_;

    std::atomic<bool> stop_flag_;
    std::condition_variable cond_;
    std::mutex mutex_;

public:
    RedisConnectionPool(size_t size, string host, int port, string password);
    ~RedisConnectionPool();
    std::shared_ptr<redisContext> get_connection();
    void stop();

private:
    redisContext *connect(const string &host, int port);
    bool auth(redisContext *context, const string &password);
    void close(redisContext *context);
};