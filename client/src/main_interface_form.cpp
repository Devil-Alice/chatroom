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
    friend_list_form_ = new FriendListForm(this);
    chat_list_from_ = new ChatListForm(this);

    ui->layout_left->addWidget(friend_list_form_);
    ui->layout_right->addWidget(chat_list_from_);

}

MainInterfaceForm::~MainInterfaceForm()
{
    delete ui;
}
