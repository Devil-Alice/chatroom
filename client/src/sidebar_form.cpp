#include "sidebar_form.h"
#include "ui_sidebar_form.h"

SidebarForm::SidebarForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SidebarForm)
{
    ui->setupUi(this);
    

    connect(ui->btn_friend, &QPushButton::clicked, this, &SidebarForm::signal_goto_friend_list);
    connect(ui->btn_message, &QPushButton::clicked, this, &SidebarForm::signal_goto_recent_message_list);

}

SidebarForm::~SidebarForm()
{
    delete ui;
}
