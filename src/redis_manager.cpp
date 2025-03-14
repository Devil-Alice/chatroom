#include "redis_manager.h"

RedisManager::RedisManager()
{
    string host = app_config["redis_server"]["host"];
    int port = std::stoi(app_config["redis_server"]["port"]);
    string password = app_config["redis_server"]["password"];

    pool_.reset(new RedisConnectionPool(5, host, port, password));
}

RedisManager::~RedisManager()
{
}

bool RedisManager::get(const string &key, string &value)
{
    std::shared_ptr<redisContext> context = pool_->get_connection();
    if (context == nullptr)
    {
        std::cout << "redis get connection failed" << std::endl;
        return false;
    }

    redisReply *reply = (redisReply *)redisCommand(context.get(), "GET %s", key.data());

    if (reply == nullptr ||
        reply->type != REDIS_REPLY_STRING)
    {
        std::cout << "redis get [" << key << "] failed" << std::endl;
        freeReplyObject(reply);
        return false;
    }

    value = reply->str;
    freeReplyObject(reply);
    std::cout << "redis get [" << key << "] success" << std::endl;
    return true;
}

bool RedisManager::set(const string &key, const string &value)
{
    std::shared_ptr<redisContext> context = pool_->get_connection();
    if (context == nullptr)
    {
        std::cout << "redis get connection failed" << std::endl;
        return false;
    }

    redisReply *reply = (redisReply *)redisCommand(context.get(), "SET %s %s", key.data(), value.data());
    if (reply == nullptr ||
        (reply->type == REDIS_REPLY_STATUS && (strcasecmp(reply->str, "ok") != 0)))
    {
        freeReplyObject(reply);
        std::cout << "redis set [" << key << "][" << value << "] failed" << std::endl;
        return false;
    }

    freeReplyObject(reply);
    std::cout << "redis set [" << key << "][" << value << "] success" << std::endl;
    return true;
}

bool RedisManager::lpush(const string &key, const string &value)
{
    std::shared_ptr<redisContext> context = pool_->get_connection();
    if (context == nullptr)
    {
        std::cout << "redis get connection failed" << std::endl;
        return false;
    }

    redisReply *reply = (redisReply *)redisCommand(context.get(), "LPUSH %s %s", key.data(), value.data());

    if (reply == nullptr || (reply->type == REDIS_REPLY_INTEGER || reply->integer <= 0))
    {
        freeReplyObject(reply);
        std::cout << "redis lpush [" << key << "][" << value << "] failed" << std::endl;
        return false;
    }

    freeReplyObject(reply);
    std::cout << "redis lpush [" << key << "][" << value << "] success" << std::endl;
    return true;
}

bool RedisManager::rpush(const string &key, const string &value)
{
    std::shared_ptr<redisContext> context = pool_->get_connection();
    if (context == nullptr)
    {
        std::cout << "redis get connection failed" << std::endl;
        return false;
    }

    redisReply *reply = (redisReply *)redisCommand(context.get(), "RPUSH %s %s", key.data(), value.data());

    if (reply == nullptr || (reply->type == REDIS_REPLY_INTEGER || reply->integer <= 0))
    {
        freeReplyObject(reply);
        std::cout << "redis rpush [" << key << "][" << value << "] failed" << std::endl;
        return false;
    }

    freeReplyObject(reply);
    std::cout << "redis rpush [" << key << "][" << value << "] success" << std::endl;
    return true;
}

bool RedisManager::lpop(const string &key, string &value)
{
    std::shared_ptr<redisContext> context = pool_->get_connection();
    if (context == nullptr)
    {
        std::cout << "redis get connection failed" << std::endl;
        return false;
    }

    redisReply *reply = (redisReply *)redisCommand(context.get(), "LPOP %s", key.data(), value.data());

    if (reply == nullptr || reply->type == REDIS_REPLY_NIL)
    {
        freeReplyObject(reply);
        std::cout << "redis lpop [" << key << "][" << value << "] failed" << std::endl;
        return false;
    }

    value = reply->str;
    freeReplyObject(reply);
    std::cout << "redis lpop [" << key << "][" << value << "] success" << std::endl;
    return true;
}

bool RedisManager::rpop(const string &key, string &value)
{
    std::shared_ptr<redisContext> context = pool_->get_connection();
    if (context == nullptr)
    {
        std::cout << "redis get connection failed" << std::endl;
        return false;
    }

    redisReply *reply = (redisReply *)redisCommand(context.get(), "RPOP %s", key.data(), value.data());

    if (reply == nullptr || reply->type == REDIS_REPLY_NIL)
    {
        freeReplyObject(reply);
        std::cout << "redis rpop [" << key << "][" << value << "] failed" << std::endl;
        return false;
    }

    value = reply->str;
    freeReplyObject(reply);
    std::cout << "redis rpop [" << key << "][" << value << "] success" << std::endl;
    return true;
}

bool RedisManager::hset(const string &key, const string &hkey, const string &value)
{
    std::shared_ptr<redisContext> context = pool_->get_connection();
    if (context == nullptr)
    {
        std::cout << "redis get connection failed" << std::endl;
        return false;
    }

    redisReply *reply = (redisReply *)redisCommand(context.get(), "HSET %s %s %s", key.data(), hkey.data(), value.data());

    if (reply == nullptr || reply->type != REDIS_REPLY_INTEGER)
    {
        freeReplyObject(reply);
        std::cout << "redis hset [" << key << "][" << hkey << "][" << value << "] failed" << std::endl;
        return false;
    }

    freeReplyObject(reply);
    std::cout << "redis hset [" << key << "][" << hkey << "][" << value << "] success" << std::endl;
    return true;
}

bool RedisManager::hset(const char *key, const char *hkey, const char *value, size_t value_len)
{
    std::shared_ptr<redisContext> context = pool_->get_connection();
    if (context == nullptr)
    {
        std::cout << "redis get connection failed" << std::endl;
        return false;
    }

    const char *argv[4];
    size_t argv_len[4];

    argv[0] = "HSET";
    argv_len[0] = 4;

    argv[1] = key;
    argv_len[1] = strlen(key);

    argv[2] = hkey;
    argv_len[2] = strlen(hkey);

    argv[3] = value;
    argv_len[3] = value_len;

    redisReply *reply = (redisReply *)redisCommandArgv(context.get(), 4, argv, argv_len);
    if (reply == nullptr || reply->type != REDIS_REPLY_INTEGER)
    {
        freeReplyObject(reply);
        std::cout << "redis hset [" << key << "][" << hkey << "][" << value << "] failed" << std::endl;
        return false;
    }

    freeReplyObject(reply);
    std::cout << "redis hset [" << key << "][" << hkey << "][" << value << "] success" << std::endl;
    return true;
}

std::string RedisManager::hget(const string &key, const string &hkey)
{
    std::shared_ptr<redisContext> context = pool_->get_connection();
    if (context == nullptr)
    {
        std::cout << "redis get connection failed" << std::endl;
        return "";
    }

    const char *argv[4];
    size_t argv_len[4];

    argv[0] = "HSET";
    argv_len[0] = 4;

    argv[1] = key.data();
    argv_len[1] = key.length();

    argv[2] = hkey.data();
    argv_len[2] = hkey.length();

    redisReply *reply = (redisReply *)redisCommandArgv(context.get(), 3, argv, argv_len);
    if (reply == nullptr || reply->type == REDIS_REPLY_NIL)
    {
        freeReplyObject(reply);
        std::cout << "redis hget [" << key << "][" << hkey << "] failed" << std::endl;
        return "";
    }

    string result = reply->str;
    freeReplyObject(reply);
    std::cout << "redis hget [" << key << "][" << hkey << "] success" << std::endl;

    return result;
}

bool RedisManager::del(const string &key)
{
    std::shared_ptr<redisContext> context = pool_->get_connection();
    if (context == nullptr)
    {
        std::cout << "redis get connection failed" << std::endl;
        return false;
    }

    redisReply *reply = (redisReply *)redisCommand(context.get(), "DEL %s", key.data());
    if (reply == nullptr || reply->type != REDIS_REPLY_INTEGER)
    {
        freeReplyObject(reply);
        std::cout << "redis del [" << key << "] failed" << std::endl;
        return false;
    }

    freeReplyObject(reply);
    std::cout << "redis del [" << key << "] success" << std::endl;

    return true;
}

bool RedisManager::exists(const string &key)
{
    std::shared_ptr<redisContext> context = pool_->get_connection();
    if (context == nullptr)
    {
        std::cout << "redis get connection failed" << std::endl;
        return false;
    }

    redisReply *reply = (redisReply *)redisCommand(context.get(), "EXISTS %s", key.data());
    if (reply == nullptr || reply->type != REDIS_REPLY_INTEGER || reply->integer == 0)
    {
        freeReplyObject(reply);
        std::cout << "redis exists [" << key << "] failed" << std::endl;
        return false;
    }

    freeReplyObject(reply);
    std::cout << "redis exists [" << key << "] success" << std::endl;
    return true;
}

void RedisManager::close()
{

}
