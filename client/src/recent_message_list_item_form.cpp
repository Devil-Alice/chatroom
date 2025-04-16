#include "recent_message_list_item_form.h"
#include "ui_recent_message_list_item_form.h"

RecentMessageListItemForm::RecentMessageListItemForm(QWidget *parent) : QWidget(parent),
                                                                        ui(new Ui::RecentMessageListItemForm)
{
    ui->setupUi(this);

    QPixmap avatar(":/image/cat.jpg");
    // avatar = avatar.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label_avatar->setFixedSize(40, 40);
    ui->label_avatar->setScaledContents(true);
    ui->label_avatar->setPixmap(avatar);
    ui->label_name->setText("alice");
    ui->label_date->setText("2024/12/31");
    ui->label_message->setText("this is a message~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~``");
}

RecentMessageListItemForm::~RecentMessageListItemForm()
{
    delete ui;
}
