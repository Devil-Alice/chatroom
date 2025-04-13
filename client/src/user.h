#ifndef USER_H
#define USER_H
#include <QString>
#include <QDate>

class User
{
public:
    User();
private:
public:
    QString uuid_;
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
