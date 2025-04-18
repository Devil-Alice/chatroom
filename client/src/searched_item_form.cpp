#include "searched_item_form.h"
#include "ui_searched_item_form.h"
#include "tcp_manager.h"

SearchedItemForm::SearchedItemForm(QString uid, QString name, QString avatar, QWidget *parent) : QWidget(parent),
                                                                                    ui(new Ui::SearchedItemForm)
{
    ui->setupUi(this);

    uid_ = uid;
    name_ = name;
    avatar_ = avatar;

    ui->label_avatar->setFixedSize(40, 40);
    ui->label_avatar->setScaledContents(true);
    if (avatar_.isEmpty())
        avatar_ = ":/image/user.png";
    ui->label_avatar->setPixmap({avatar_});

    ui->label_name->setText(name_);

    ui->pushButton->setText("添加");


    connect(ui->pushButton, &QPushButton::clicked, this, &SearchedItemForm::slot_send_friend_apply);
}

SearchedItemForm::~SearchedItemForm()
{
    delete ui;
}

void SearchedItemForm::slot_send_friend_apply()
{
    QJsonObject json_obj;
    json_obj["from_uid"] = self_info->uid_;
    json_obj["to_uid"] = this->uid_;
    json_obj["token"] = self_info->token_;
    QJsonDocument json_doc(json_obj);

    emit TcpManager::instance()->signal_send_message(REQUEST_ID::FRIEND_APPLY, json_doc.toJson());

}
