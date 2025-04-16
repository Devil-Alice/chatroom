#include "friend_list_item_form.h"
#include "ui_friend_list_item_form.h"

FriendListItemForm::FriendListItemForm(QWidget *parent) : QWidget(parent),
                                                          ui(new Ui::FriendListItemForm)
{
    ui->setupUi(this);

    QPixmap avatar(":/image/cat.jpg");
    ui->label_avatar->setFixedSize(40, 40);
    ui->label_avatar->setScaledContents(true);
    ui->label_avatar->setScaledContents(true);
    ui->label_avatar->setPixmap(avatar);

    ui->label_name->setText("alice");
}

FriendListItemForm::~FriendListItemForm()
{
    delete ui;
}
