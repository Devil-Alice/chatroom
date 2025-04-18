#include "chat_form.h"
#include "ui_chat_form.h"

ChatForm::ChatForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatForm)
{
    ui->setupUi(this);

    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // 隐藏垂直滚动条
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // 隐藏水平滚动条

    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 0);

    add_message(0);
    add_message(1);
    

}

ChatForm::~ChatForm()
{
    delete ui;
}

void ChatForm::add_message(int role)
{

    ChatMessageItemForm *message = new ChatMessageItemForm(role, this);
    ui->layout_chat_message->addWidget(message);
}
