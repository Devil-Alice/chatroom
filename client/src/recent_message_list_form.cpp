#include "recent_message_list_form.h"
#include "ui_recent_message_list_form.h"

RecentMessageListForm::RecentMessageListForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecentMessageListForm)
{
    ui->setupUi(this);
}

RecentMessageListForm::~RecentMessageListForm()
{
    delete ui;
}
