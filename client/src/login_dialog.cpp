#include "login_dialog.h"
#include "ui_login_dialog.h"

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent),
                                            ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    ui->text_password->setEchoMode(QLineEdit::Password);

    init_response_handler();

    // 当点击click时，发送signal_goto_register信号
    connect(ui->btn_register, &QPushButton::clicked, this, &LoginDialog::signal_goto_register);

    // 登录按钮的事件
    connect(ui->btn_login, &QPushButton::clicked, this, &LoginDialog::slot_user_login);

    // 处理httpmanager发送来的信号
    connect(HttpManager::instance().get(), &HttpManager::signal_login_request_finished, this, &LoginDialog::slot_login_request_finished);

    // 处理tcp连接信号
    connect(this, &LoginDialog::signal_connect_to_server, TcpManager::instance().get(), &TcpManager::slot_connect_to_server);

    // 处理连接完成的信号
    connect(TcpManager::instance().get(), &TcpManager::signal_connection_status, this, &LoginDialog::slot_handle_connection_status);

    // 处理chat_login的tcp回复
    connect(TcpManager::instance().get(), &TcpManager::signal_chat_login_finished, this, &LoginDialog::slot_chat_login_finished);
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

        // 确保 data 部分存在并解析
        if (!json_obj.contains("data") || !json_obj["data"].isObject())
        {
            QMessageBox::information(this, "Error", "Invalid response data.");
            return;
        }

        QJsonObject json_obj_data = json_obj["data"].toObject();

        // 登录成功后，尝试连接tcp服务器
        ConnectoinInfo info;
        info.uid = json_obj_data["uid"].toString();
        info.host= json_obj_data["host"].toString();
        info.port= json_obj_data["port"].toInt();
        info.token= json_obj_data["token"].toString();

        // 记录token和uid
        self_info->uid_ = info.uid;
        self_info->token_ = info.token;
        self_info->name_ = json_obj_data["name"].toString();

        emit signal_connect_to_server(info);
        return; });
}

void LoginDialog::slot_user_login()
{

    // test code
    // emit signal_goto_main_interface();
    // return;


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
    json_obj["password"] = md5_encrypt(password);
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

void LoginDialog::slot_handle_connection_status(bool success)
{
    if (!success)
    {
        qDebug() << "login tcp connection failed";
        return;
    }

    QJsonObject json_obj;
    json_obj["phone"] = ui->text_phone->text();
    json_obj["uid"] = self_info->uid_;
    json_obj["token"] = self_info->token_;

    QJsonDocument json_doc(json_obj);
    QByteArray data = json_doc.toJson();

    emit TcpManager::instance() -> signal_send_message(REQUEST_ID::CHAT_LOGIN, data);

    return;
}

void LoginDialog::slot_chat_login_finished(QJsonObject json_data)
{

    int status = json_data["status"].toInt();
    QString message = json_data["message"].toString();
    if (status != MY_STATUS_CODE::SUCCESS)
    {
        QMessageBox::information(this, "info", message);
        return;
    }

    QJsonObject data = json_data["data"].toObject();

    //跳转到主页
    emit signal_goto_main_interface();

    return;
}
