#include "register_dialog.h"
#include "ui_register_dialog.h"
#include "http_manager.h"

RegisterDialog::RegisterDialog(QWidget *parent) : QDialog(parent),
                                                  ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    
    
    // 测试信息：-----------------
    ui->text_name->setText("alice");
    ui->text_password->setText("123");
    ui->text_confirm_password->setText("123");
    ui->text_phone->setText("18912345678");
    // 测试信息：++++++++++++++++++
    
    
    // 初始化定时器
    timer_ = new QTimer(this);

    // 设置定时器事件
    connect(timer_, &QTimer::timeout, this, [this](){
        if (time_count_ <= 0)
        {   
            ui->btn_get_verify_code->setText("发送");
            timer_->stop();
            ui->btn_get_verify_code->setEnabled(true);
            return;
        }

        time_count_--;
        ui->btn_get_verify_code->setText("发送(" + QString::number(time_count_) +"s)");
        
    });


    // 初始化网络处理事件
    init_response_handler();

    // 设置密码不可见
    ui->text_password->setEchoMode(QLineEdit::Password);
    ui->text_confirm_password->setEchoMode(QLineEdit::Password);

    // 注册用户输入检查事件
    connect(ui->text_name, &QLineEdit::editingFinished, this,               &slot_check_name);
    connect(ui->text_phone, &QLineEdit::editingFinished, this,              &slot_check_phone);
    connect(ui->text_verify_code, &QLineEdit::editingFinished, this,        &slot_check_verify_code);
    connect(ui->text_password, &QLineEdit::editingFinished, this,           &slot_check_password);
    connect(ui->text_confirm_password, &QLineEdit::editingFinished, this,   &slot_check_confirm_password);

    // 注册界面收到网络响应后的事件
    connect(HttpManager::instance().get(), &HttpManager::signal_resigter_request_finished, this, &RegisterDialog::slot_register_request_finished);

    // 返回按钮的点击事件
    connect(ui->btn_return, &QPushButton::clicked, this, [this](){
        emit signal_return();
    });
    // 获取验证码的按钮点击事件
    connect(ui->btn_get_verify_code, &QPushButton::clicked, this, &RegisterDialog::slot_get_verify_code);
    // 注册按钮的点击事件
    connect(ui->btn_register, &QPushButton::clicked, this, &RegisterDialog::slot_register_user);
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
    http_response_handler_[request_id](json_obj);

    return;
}

void RegisterDialog::slot_check_name()
{
    if (ui->text_name->text().isEmpty())
    {
        add_tip("name", "用户名不能为空");
        return;
    }

    remove_tip("name");
}

void RegisterDialog::slot_check_phone()
{
    if (ui->text_phone->text().isEmpty())
    {
        add_tip("phone", "手机号不能为空");
        return;
    }

    // 利用正则表达式验证手机号
    QString phone = ui->text_phone->text();
    QRegularExpression regex("^1[3-9]\\d{9}$");
    bool match = regex.match(phone).hasMatch();
    // 验证失败
    if (!match)
    {
        add_tip("phone", "手机号不正确");
        return;
    }

    remove_tip("phone");

}

void RegisterDialog::slot_check_verify_code()
{
    if (ui->text_verify_code->text().isEmpty())
    {
        add_tip("verify_code", "验证码不能为空");
        return;
    }
    remove_tip("verify_code");
}

void RegisterDialog::slot_check_password()
{
    if (ui->text_password->text().isEmpty())
    {
        add_tip("password", "密码不能为空");
        return;
    }

    // 检查两次密码是否一致
    if (!(ui->text_password->text() == ui->text_confirm_password->text()))
    {
        add_tip("same_password", "两次密码不一致");
        return; 
    }

    remove_tip("same_password");
    remove_tip("password");
    
}

void RegisterDialog::slot_check_confirm_password()
{
    if (ui->text_confirm_password->text().isEmpty())
    {
        add_tip("confirm_password", "确认密码不能为空");
        return;
    }

    // 检查两次密码是否一致
    if (!(ui->text_password->text() == ui->text_confirm_password->text()))
    {
        add_tip("same_password", "两次密码不一致");
        return; 
    }

    remove_tip("same_password");
    remove_tip("confirm_password");
}

void RegisterDialog::init_response_handler()
{
    http_response_handler_[REQUEST_ID::GET_VERIFY_CODE] = [this](QJsonObject &json_obj)
    {
        int error_code = json_obj["status"].toInt();
        if (error_code != MY_STATUS_CODE::SUCCESS)
        {
            show_register_msg(json_obj["message"].toString(), "error");
            return;
        }

        // 禁用按钮
        ui->btn_get_verify_code->setEnabled(false);
        ui->btn_get_verify_code->setText("发送(60s)");
        // 启用定时器
        timer_->setInterval(1000);
        time_count_ = 60;
        timer_->start();

        show_register_msg(json_obj["message"].toString(), "success");
        return;
    };

    http_response_handler_[REQUEST_ID::REGISTER_USER] = [this](QJsonObject &json_obj)
    {
        int error_code = json_obj["status"].toInt();
        if (error_code != MY_STATUS_CODE::SUCCESS)
        {
            show_register_msg(json_obj["message"].toString(), "error");
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

void RegisterDialog::show_tip()
{
    if (tips_.isEmpty())
    {
        ui->label_register_msg->clear();
        return;
    }
    QString tip_msg = tips_.first();
    show_register_msg(tip_msg, "error");

}

void RegisterDialog::add_tip(QString key, QString value)
{
    tips_[key] = value;
    show_tip();
}

void RegisterDialog::remove_tip(QString key)
{
    tips_.remove(key);
    show_tip();
}

void RegisterDialog::slot_get_verify_code()
{
    // 存在手机号错误
    if (tips_.find("phone") != tips_.end())
    {
        show_tip();
        return;
    }
    
    QString phone = ui->text_phone->text();

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

void RegisterDialog::slot_register_user()
{
    if (!tips_.isEmpty())
    {
        show_tip();
        return;
    }

    QJsonObject json_obj;
    json_obj["name"] = ui->text_name->text();
    json_obj["phone"] = ui->text_phone->text();
    json_obj["verify_code"] = ui->text_verify_code->text();
    json_obj["password"] = md5_encrypt(ui->text_password->text());

    HttpManager::instance()->post_request(QUrl(gate_url_prefix + "/user"), json_obj, MODULE::REGISTER, REQUEST_ID::REGISTER_USER);
    return;

}
