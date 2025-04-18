#ifndef USER_H
#define USER_H
#include <QString>
#include <QDate>
#include <memory>

class User
{
public:
    User();
private:
public:
    QString uid_;
    QString name_;
    QString avatar_;

};

class SelfInfo : public User
{
private:
public:
    QString token_;
    QString server_host_;
    int server_port_;
};

extern std::shared_ptr<SelfInfo> self_info;

class Friend : public User
{
private:
public:
    QString nickname_;

};

class ChatMessage
{
private:
public:
    User sender_;
    QDate date_;
    QString message_;
};

#endif // USER_H
