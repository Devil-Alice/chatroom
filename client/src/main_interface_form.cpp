#include "main_interface_form.h"
#include "ui_main_interface_form.h"

MainInterfaceForm::MainInterfaceForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainInterfaceForm)
{
    ui->setupUi(this);

    // 设置拉伸因子
    ui->splitter_x->setStretchFactor(0, 0);
    ui->splitter_x->setStretchFactor(1, 0);
    ui->splitter_x->setStretchFactor(2, 1);

    ui->splitter_y->setStretchFactor(0, 0);
    ui->splitter_y->setStretchFactor(1, 1);

    // 初始化成员变量
    sidebar_form_ = new SidebarForm(this);
    recent_message_list_form_ = new RecentMessageListForm(this);
    friend_list_form_ = new FriendListForm(this);
    chat_list_from_ = new ChatForm(this);

    ui->layout_sidebar->addWidget(sidebar_form_);
    ui->layout_left->addWidget(recent_message_list_form_);
    // recent_message_list_form_->hide();
    ui->layout_left->addWidget(friend_list_form_);
    friend_list_form_->hide();
    ui->layout_right->addWidget(chat_list_from_);



    // 窗口切换事件
    connect(sidebar_form_, &SidebarForm::signal_goto_recent_message_list, this, &MainInterfaceForm::show_recent_message_list);
    connect(sidebar_form_, &SidebarForm::signal_goto_friend_list, this, &MainInterfaceForm::show_friend_list);

}

MainInterfaceForm::~MainInterfaceForm()
{
    delete ui;
}

void MainInterfaceForm::show_recent_message_list()
{
    friend_list_form_->hide();
    recent_message_list_form_->show();
}

void MainInterfaceForm::show_friend_list()
{
    recent_message_list_form_->hide();
    friend_list_form_->show();
}


