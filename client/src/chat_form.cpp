#include "chat_form.h"
#include "ui_chat_list_form.h"

ChatListForm::ChatListForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatListForm)
{
    ui->setupUi(this);
}

ChatListForm::~ChatListForm()
{
    delete ui;
}
