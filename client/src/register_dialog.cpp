#include "register_dialog.h"
#include "ui_register_dialog.h"
#include "http_manager.h"

RegisterDialog::RegisterDialog(QWidget *parent) : QDialog(parent),
                                                  ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);

    init_request_handler();

    ui->text_password->setEchoMode(QLineEdit::Password);
    ui->text_confirm_password->setEchoMode(QLineEdit::Password);

    // 注册界面收到网络响应后的事件
    connect(HttpManager::instance().get(), &HttpManager::signal_resigter_request_finished, this, &RegisterDialog::slot_register_request_finished);
    
    // 获取验证码的按钮点击事件
    connect(ui->btn_get_verify_code, &QPushButton::clicked, this, &RegisterDialog::slot_get_verify_code);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::slot_register_request_finished(MY_STATUS_CODE code, REQUEST_ID request_id, QString response)
{
    if (code != MY_STATUS_CODE::SUCCESS)
    {
        show_register_msg("注册出错", "error");
        return;
    }

    // 解析json
    QJsonDocument json_doc = QJsonDocument::fromJson(response.toUtf8());
    if (json_doc.isNull() || !json_doc.isObject())
    {
        show_register_msg("json解析错误", "error");
        return;
    }

    QJsonObject json_obj = json_doc.object();

    // todo: 判断返回的内容
    http_request_handler_[request_id](json_obj);

    return;
}

void RegisterDialog::init_request_handler()
{
    http_request_handler_[REQUEST_ID::GET_VERIFY_CODE] = [this](QJsonObject &json_obj)
    {
        int error_code = json_obj["status"].toInt();
        if (error_code != MY_STATUS_CODE::SUCCESS)
        {
            show_register_msg("发送失败，请检查信息是否正确", "error");
            return;
        }

        show_register_msg(json_obj["message"].toString(), "success");
        return;
    };
}

void RegisterDialog::show_register_msg(QString msg, QString status)
{
    ui->label_register_msg->setProperty("status", status);
    refresh_style(ui->label_register_msg);

    ui->label_register_msg->setText(msg);
    return;
}

void RegisterDialog::slot_get_verify_code()
{
    // 利用正则表达式验证手机号
    QString phone = ui->text_phone->text();
    QRegularExpression regex("^1[3-9]\\d{9}$");
    bool match = regex.match(phone).hasMatch();
    // 验证失败
    if (!match)
    {
        show_register_msg("手机号不正确", "error");
        return;
    }

    // 验证成功
    // 向服务器请求发送验证码
    // 创建url
    QString url_str = gate_url_prefix + "/verify_code";
    QUrl url(url_str);

    // 创建json数据
    QJsonObject json_obj;
    json_obj["phone"] = phone;

    std::shared_ptr<HttpManager> http_manager(HttpManager::instance());

     http_manager->post_request(url, json_obj, MODULE::REGISTER, REQUEST_ID::GET_VERIFY_CODE);

    return;
}
