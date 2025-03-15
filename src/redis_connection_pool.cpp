#include "redis_connection_pool.h"

RedisConnectionPool::RedisConnectionPool(size_t size, string host, int port, string password)
    : pool_size_(size), host_(host), port_(port)
{
    stop_flag_ = false;

    for (size_t i = 0; i < pool_size_; i++)
    {
        redisContext *context = connect(host, port);
        if (!password.empty())
            auth(context, password);
        connections_.push(context);
    }
}

RedisConnectionPool::~RedisConnectionPool()
{
    stop();
}

std::shared_ptr<redisContext> RedisConnectionPool::get_connection()
{
    std::unique_lock<std::mutex> locker(mutex_);
    cond_.wait(locker, [this]() -> bool
               { return stop_flag_ || !connections_.empty(); });

    if (stop_flag_)
        return nullptr;

    auto conn = connections_.front();
    connections_.pop();

    std::shared_ptr<redisContext> conn_ptr(conn, [this](redisContext *redis_conn)
                                           {
        mutex_.lock();
        connections_.push(redis_conn);
        mutex_.unlock();
        cond_.notify_one(); });

    return conn_ptr;
}

void RedisConnectionPool::stop()
{
    std::lock_guard<std::mutex> locker(mutex_);
    stop_flag_ = true;
    cond_.notify_all();
    while (!connections_.empty())
    {
        close(connections_.front());
        connections_.pop();
    }
}

redisContext *RedisConnectionPool::connect(const string &host, int port)
{
    redisContext *context = redisConnect(host.data(), port);
    if (context == nullptr)
    {
        return nullptr;
    }

    if (context->err)
    {
        std::cout << "redis connection error: " << context->err << std::endl;
        return nullptr;
    }

    return context;
}

bool RedisConnectionPool::auth(redisContext *context, const string &password)
{
    redisReply *reply;
    reply = (redisReply *)redisCommand(context, "AUTH %s", password.data());

    if (reply == nullptr || reply->type == REDIS_REPLY_ERROR)
    {
        freeReplyObject(reply);
        std::cout << "redis auth failed" << std::endl;
        return false;
    }

    freeReplyObject(reply);
    std::cout << "redis auth success" << std::endl;
    return true;
}

void RedisConnectionPool::close(redisContext *context)
{
    if (context)
        redisFree(context);
}