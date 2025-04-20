#ifndef USER_H
#define USER_H
#include <QString>
#include <QDate>
#include <memory>
#include <QJsonObject>

class User
{
public:
    User();
    User(QString uid, QString name, QString avatar);

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

    QJsonObject get_basic_info_json();
};

extern std::shared_ptr<SelfInfo> self_info;

class FriendApply
{
public:
    User from_user_;
    User to_user_;
    QString remark_name_;
    QString apply_message_;
    int status_;

    FriendApply() {};

    FriendApply(User from_user, User to_user, QString remark_name, QString apply_message, int status);
};

class Friend : public User
{
private:
public:
    QString remark_name_;
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
