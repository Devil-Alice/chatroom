#include "login_dialog.h"
#include "ui_login_dialog.h"

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent),
                                            ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    ui->text_password->setEchoMode(QLineEdit::Password);

    // 当点击click时，发送signal_goto_register信号
    connect(ui->btn_register, &QPushButton::clicked, this, &LoginDialog::signal_goto_register);

    // 登录按钮的事件
    connect(ui->btn_login, &QPushButton::clicked, this, &LoginDialog::slot_user_login);

    // 处理httpmanager发送来的信号
    connect(HttpManager::instance().get(), &HttpManager::signal_login_request_finished, this, &LoginDialog::slot_login_request_finished);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::init_response_handler()
{
    http_response_handler_.insert(REQUEST_ID::USER_LOGIN, [this](QJsonObject &json_obj)
                                  {
        int error_code = json_obj["status"].toInt();
        if (error_code != MY_STATUS_CODE::SUCCESS)
        {
            QMessageBox::information(this, "info",json_obj["message"].toString());
            return;
        }
        
        QMessageBox::information(this, "info",json_obj["message"].toString());
        return; });
}

void LoginDialog::slot_user_login()
{
    QString phone = ui->text_phone->text();
    QString password = ui->text_password->text();

    if (phone.isEmpty())
    {
        QMessageBox::information(this, "info", "手机号不能为空");
        return;
    }

    if (password.isEmpty())
    {
        QMessageBox::information(this, "info", "密码不能为空");
        return;
    }


    QJsonObject json_obj;
    json_obj["phone"] = phone;
    json_obj["password"] = password;
    HttpManager::instance()->post_request(QUrl(gate_url_prefix + "/login"), json_obj, MODULE::LOGIN, REQUEST_ID::USER_LOGIN);

    return;
}

void LoginDialog::slot_login_request_finished(MY_STATUS_CODE code, REQUEST_ID request_id, QString response)
{

    if (code != MY_STATUS_CODE::SUCCESS)
    {
        QMessageBox::information(this, "info", "响应错误");
        return;
    }

    // 解析json
    QJsonDocument json_doc = QJsonDocument::fromJson(response.toUtf8());
    if (json_doc.isNull() || !json_doc.isObject())
    {
        QMessageBox::information(this, "info", "json解析错误");
        return;
    }

    QJsonObject json_obj = json_doc.object();
    http_response_handler_[request_id](json_obj);

    return;
}