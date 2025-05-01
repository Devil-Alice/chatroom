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
    connect(friend_list_form_, &FriendListForm::slot_goto_friend_apply_list, this, &MainInterfaceForm::slot_goto_friend_apply_list);
    
    // 处理收到好友请求的提醒
    connect(TcpManager::instance().get(), &TcpManager::signal_notify_receive_friend_apply, this, &MainInterfaceForm::slot_notify_receive_friend_apply);
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

void MainInterfaceForm::slot_notify_receive_friend_apply(QJsonObject json_data)
{
    // 设置角标
    sidebar_form_->set_friend_notify_visible(true);

    // 将新的好友请求添加进scrolllist中
    friend_apply_list_form_->add_friend_apply_item_from_json(json_data);
}
