#include "friend_list_form.h"
#include "ui_friend_list_form.h"

FriendListForm::FriendListForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendListForm)
{
    ui->setupUi(this);



}

FriendListForm::~FriendListForm()
{
    delete ui;
}
