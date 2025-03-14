#pragma once
#include "global.h"
#include "redis_connection_pool.h"

class RedisManager : public Singleton<RedisManager>
{
    friend class Singleton<RedisManager>;
    using string = std::string;

private:
    // redisContext类似connection
    // redisContext *context_;
    // redisReply *reply_;
    std::unique_ptr<RedisConnectionPool> pool_; 

    RedisManager();

public:
    ~RedisManager();
    
    bool get(const string &key, string &value);
    bool set(const string &key, const string &value);
    bool lpush(const string &key, const string &value);
    bool rpush(const string &key, const string &value);
    bool lpop(const string &key, string &value);
    bool rpop(const string &key, string &value);
    bool hset(const string &key, const string &hkey, const string &value);
    bool hset(const char *key, const char *hkey, const char *value, size_t value_len);
    string hget(const string &key, const string &hkey);
    bool del(const string &key);
    bool exists(const string &key);
    void close();
};