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
    search_bar_form_ = new SearchBarForm(this);
    recent_message_list_form_ = new RecentMessageListForm(this);
    friend_list_form_ = new FriendListForm(this);
    friend_list_form_->hide();
    chat_form_ = new ChatForm(this);
    scroll_list_form_ = new ScrollListForm(this);
    scroll_list_form_->hide();

    ui->layout_sidebar->addWidget(sidebar_form_);
    ui->layout_top->addWidget(search_bar_form_);
    ui->layout_left->addWidget(recent_message_list_form_);
    ui->layout_left->addWidget(friend_list_form_);
    ui->layout_right->addWidget(chat_form_);
    ui->layout_right->addWidget(scroll_list_form_);




    // 窗口切换事件
    connect(sidebar_form_, &SidebarForm::signal_goto_recent_message_list, this, &MainInterfaceForm::slot_goto_recent_message_list);
    connect(sidebar_form_, &SidebarForm::signal_goto_friend_list, this, &MainInterfaceForm::slot_goto_friend_list);
    connect(search_bar_form_, &SearchBarForm::signal_goto_search_result_list, this, &MainInterfaceForm::slot_goto_search_result_list);

}

MainInterfaceForm::~MainInterfaceForm()
{
    delete ui;
}

void MainInterfaceForm::slot_goto_recent_message_list()
{
    friend_list_form_->hide();
    recent_message_list_form_->show();
}

void MainInterfaceForm::slot_goto_friend_list()
{
    recent_message_list_form_->hide();
    friend_list_form_->show();
}

void MainInterfaceForm::slot_goto_search_result_list()
{
    chat_form_->hide();
    scroll_list_form_->show();
}
