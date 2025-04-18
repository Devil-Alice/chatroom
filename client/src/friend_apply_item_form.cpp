#include "friend_apply_item_form.h"
#include "ui_friend_apply_item_form.h"

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
            ui->btn_reject->hide();
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
        ui->btn_reject->hide();
    }


    ui->label_apply_message->setText(friend_apply.apply_message_);
}

FriendApplyItemForm::~FriendApplyItemForm()
{
    delete ui;
}
