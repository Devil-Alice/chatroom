#include "sidebar_form.h"
#include "ui_sidebar_form.h"

SidebarForm::SidebarForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SidebarForm)
{
    ui->setupUi(this);
    
    // 默认隐藏红点
    set_message_notify_visible(false);
    set_friend_notify_visible(false);

    ui->label_name->setText(self_info->name_);

    connect(ui->btn_friend, &QPushButton::clicked, this, &SidebarForm::signal_goto_friend_list);
    connect(ui->btn_message, &QPushButton::clicked, this, &SidebarForm::signal_goto_recent_message_list);

}

SidebarForm::~SidebarForm()
{
    delete ui;
}

void SidebarForm::set_message_notify_visible(bool is_visible)
{
    ui->label_notify_point_message->setVisible(is_visible);
}

void SidebarForm::set_friend_notify_visible(bool is_visible)
{
    ui->label_notify_point_friend->setVisible(is_visible);
}
