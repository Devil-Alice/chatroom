#include "friend_apply_form.h"
#include "ui_friend_apply_form.h"
#include "tcp_manager.h"

FriendApplyForm::FriendApplyForm(QString to_uid, QString name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendApplyForm)
{
    ui->setupUi(this);

    to_uid_ = to_uid;
    name_ = name;

    ui->label_name->setText(name_);

    connect(ui->btn_send, &QPushButton::clicked, this, &FriendApplyForm::send_apply);
}

FriendApplyForm::~FriendApplyForm()
{
    delete ui;
}

void FriendApplyForm::send_apply()
{

    QString remark_name = ui->text_remark_name->text();
    QString apply_message = ui->text_apply_message->toPlainText();

    QJsonObject json_obj;
    json_obj["from_uid"] = self_info->uid_;
    json_obj["token"] = self_info->token_;
    json_obj["to_uid"] = this->to_uid_;
    json_obj["remark_name"] = remark_name;
    json_obj["apply_message"] = apply_message;

    QJsonDocument json_doc(json_obj);

    emit TcpManager::instance()->signal_send_message(REQUEST_ID::SNED_FRIEND_APPLY, json_doc.toJson());
}
