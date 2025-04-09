#ifndef GLOBAL_H
#define GLOBAL_H
#include <QWidget>
#include <QStyle>
#include <iostream>
#include <memory>
#include <functional>
#include <QApplication>
#include <QObject>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QFile>
#include <QRegularExpression>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QTcpSocket>
#include <QDesktopWidget>
#include "singleton.h"
#include "config_manager.h"

extern QString gate_url_prefix;

void refresh_style(QWidget *widget);

QString md5_encrypt(const QString &data);

void move_to_screen_center(QWidget *window);

// MODULE和REQUEST_ID: 服务网络请求的回调, 标记响应的数据是当时哪个模块的哪个功能
enum MODULE
{
    REGISTER = 1001,
    LOGIN = 1002
};

enum REQUEST_ID
{
    UNKNOWN = 1000,
    GET_VERIFY_CODE = 1001, //获取验证码
    REGISTER_USER = 1002, //用户注册
    USER_LOGIN = 1003, // 用户登录
    CHAT_LOGIN = 1004  //聊天服务器登录

};

enum MY_STATUS_CODE
{
    SUCCESS = 0,
    JSON_ERROR = 1001,
    RPC_RAILED = 1002,
    TIMEOUT = 1003,
    VERIFY_CODE_EXPIRED = 1004,
    VERIFY_CODE_ERROR = 1005,
    USER_EXIST = 1006,
    PHONE_INVALID = 1007,
    PASSWORD_ERROR = 1008,
    NETWORK_FAILED = 1009
};


#endif // GLOBAL_H
