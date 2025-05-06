#include "main_interface_form.h"
#include "ui_main_interface_form.h"

MainInterfaceForm::MainInterfaceForm(QWidget *parent) : QWidget(parent),
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
    friend_apply_list_form_ = new FriendApplyListForm(this);
    friend_apply_list_form_->hide();
    searched_list_form_ = new SearchedListForm(this);
    searched_list_form_->hide();

    ui->layout_sidebar->addWidget(sidebar_form_);
    ui->layout_top->addWidget(search_bar_form_);
    ui->layout_left->addWidget(recent_message_list_form_);
    ui->layout_left->addWidget(friend_list_form_);
    ui->layout_right->addWidget(chat_form_);
    ui->layout_right->addWidget(friend_apply_list_form_);
    ui->layout_right->addWidget(searched_list_form_);

    // 窗口切换事件
    connect(sidebar_form_, &SidebarForm::signal_goto_recent_message_list, this, &MainInterfaceForm::slot_goto_recent_message_list);
    connect(sidebar_form_, &SidebarForm::signal_goto_friend_list, this, &MainInterfaceForm::slot_goto_friend_list);
    connect(search_bar_form_, &SearchBarForm::signal_goto_search_result_list, this, &MainInterfaceForm::slot_goto_search_result_list);
    connect(friend_list_form_, &FriendListForm::signal_goto_friend_apply_list, this, &MainInterfaceForm::slot_goto_friend_apply_list);

    // 查询好友申请后，设置未读数字
    connect(friend_apply_list_form_, &FriendApplyListForm::signal_set_unhandled_apply_nums, this, &MainInterfaceForm::slot_set_unhandled_apply_num);

    // 初始化完成后，需要获取一些基本信息
    friend_list_form_->query_friend_applys();
}

MainInterfaceForm::~MainInterfaceForm()
{
    delete ui;
}

void MainInterfaceForm::hide_all_widgets_in_layout(QLayout *layout)
{
    if (!layout)
        return;

    // 遍历所有控件并隐藏
    for (int i = 0; i < layout->count(); ++i)
    {
        QLayoutItem *item = layout->itemAt(i);
        if (item)
        {
            QWidget *widget = item->widget();
            if (widget)
            {
                widget->hide();
            }
        }
    }
}

void MainInterfaceForm::slot_goto_recent_message_list()
{
    hide_all_widgets_in_layout(ui->layout_left);
    recent_message_list_form_->show();

    hide_all_widgets_in_layout(ui->layout_right);
    chat_form_->show();
}

void MainInterfaceForm::slot_goto_friend_list()
{
    hide_all_widgets_in_layout(ui->layout_left);
    friend_list_form_->show();
    hide_all_widgets_in_layout(ui->layout_right);
}

void MainInterfaceForm::slot_goto_friend_apply_list()
{
    hide_all_widgets_in_layout(ui->layout_right);
    friend_apply_list_form_->show();
}

void MainInterfaceForm::slot_goto_search_result_list()
{
    hide_all_widgets_in_layout(ui->layout_right);
    searched_list_form_->show();
}

void MainInterfaceForm::slot_set_unhandled_apply_num(int num)
{
    // 设置角标
    sidebar_form_->set_friend_notify_visible(num);
    friend_list_form_->set_unhandled_friend_apply_num(friend_apply_list_form_->get_unhandled_apply_num());
}
