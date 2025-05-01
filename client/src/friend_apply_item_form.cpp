#include "friend_apply_item_form.h"
#include "ui_friend_apply_item_form.h"
#include "tcp_manager.h"

FriendApplyItemForm::FriendApplyItemForm(FriendApply friend_apply, QWidget *parent) : QWidget(parent),
                                                                                      ui(new Ui::FriendApplyItemForm)
{
    ui->setupUi(this);
    setFixedHeight(70);
    ui->label_avatar->setFixedSize(40, 40);
    ui->label_avatar->setScaledContents(true);


    friend_apply_ = friend_apply;
    User from_user = friend_apply.from_user_;
    User to_user = friend_apply.to_user_;
    int status = friend_apply.status_;
    QString status_str;
    if (status == 0)
        status_str = "待处理";
    else if (status == 1)
        status_str = "已通过";
    else if (status == 2)
        status_str = "已拒绝";
    ui->label_status->setText(status_str);

    // 判断是自己发的，还是别人发过来的
    if (to_user.uid_ == self_info->uid_)
    {
        // 别人发的
        QString avatar = from_user.avatar_;
        if (avatar.isEmpty())
            avatar = ":/image/user.png";
        ui->label_avatar->setPixmap({avatar});
        ui->label_name->setText(from_user.name_ + " 发来一条好友申请");

        if (status == 0)
        {
            // 别人发的，并且是未处理，显示按钮，隐藏状态
            ui->label_status->hide();
        }
        else
        {
            ui->btn_accept->hide();
            ui->btn_accept->setEnabled(false);
            ui->btn_reject->hide();
            ui->btn_reject->setEnabled(false);
        }
    }
    else if (from_user.uid_ == self_info->uid_)
    {
        // 自己发的
        QString avatar = to_user.avatar_;
        if (avatar.isEmpty())
            avatar = ":/image/user.png";
        ui->label_avatar->setPixmap({avatar});
        ui->label_name->setText(to_user.name_ + " 的好友申请已发送");

        ui->btn_accept->hide();
        ui->btn_accept->setEnabled(false);
        ui->btn_reject->hide();
        ui->btn_reject->setEnabled(false);
    }


    ui->label_apply_message->setText(friend_apply.apply_message_);




    // 处理好友申请的动作
    connect(ui->btn_accept, &QPushButton::clicked, this, &FriendApplyItemForm::slot_btn_accpet_clicked);
    connect(ui->btn_reject, &QPushButton::clicked, this, &FriendApplyItemForm::slot_btn_reject_clicked);
}

FriendApplyItemForm::~FriendApplyItemForm()
{
    delete ui;
}

std::shared_ptr<FriendApplyItemForm> FriendApplyItemForm::parse_from_json(QJsonObject json_obj)
{
    QJsonObject from_user_json = json_obj["from_user"].toObject();
    QJsonObject to_user_json = json_obj["to_user"].toObject();

    QString from_uid = from_user_json["uid"].toString();
    QString from_name = from_user_json["name"].toString();
    QString from_avatar = from_user_json["avatar"].toString();
    User from_user(from_uid, from_name, from_avatar);

    QString to_uid = to_user_json["uid"].toString();
    QString to_name = to_user_json["name"].toString();
    QString to_avatar = to_user_json["avatar"].toString();
    User to_user(to_uid, to_name, to_avatar);

    QString remark_name = json_obj["remark_name"].toString();
    QString apply_message = json_obj["apply_message"].toString();
    int status = json_obj["status"].toInt();
    FriendApply friend_apply(from_user, to_user, remark_name, apply_message, status);

    auto widget = std::make_shared<FriendApplyItemForm>(friend_apply);
    return widget;
}

FriendApply FriendApplyItemForm::get_friend_apply()
{
    return friend_apply_;
}

void FriendApplyItemForm::slot_btn_accpet_clicked()
{
    QJsonObject json_obj = self_info->get_basic_info_json();
    json_obj["from_uid"] = friend_apply_.from_user_.uid_;
    json_obj["to_uid"] = friend_apply_.to_user_.uid_;
    json_obj["remark_name"] = friend_apply_.remark_name_;
    json_obj["status"] = 1;

    QJsonDocument json_doc(json_obj);
    emit TcpManager::instance()->signal_send_message(REQUEST_ID::HANDLE_FRIEND_APPLY, json_doc.toJson());
}


void FriendApplyItemForm::slot_btn_reject_clicked()
{

    QJsonObject json_obj = self_info->get_basic_info_json();
    json_obj["from_uid"] = friend_apply_.from_user_.uid_;
    json_obj["to_uid"] = friend_apply_.to_user_.uid_;
    json_obj["remark_name"] = friend_apply_.remark_name_;
    json_obj["status"] = 2;

    QJsonDocument json_doc(json_obj);
    emit TcpManager::instance()->signal_send_message(REQUEST_ID::HANDLE_FRIEND_APPLY, json_doc.toJson());
}
