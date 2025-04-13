#include "recent_message_list_form.h"
#include "ui_recent_message_list_form.h"

RecentMessageListForm::RecentMessageListForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecentMessageListForm)
{
    ui->setupUi(this);

    setMinimumWidth(250);

    QListWidgetItem *item = new QListWidgetItem();
    RecentMessageListItemForm *msg_item = new RecentMessageListItemForm(this);
    item->setSizeHint(msg_item->sizeHint());
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, msg_item);


    QListWidgetItem *item2 = new QListWidgetItem();
    RecentMessageListItemForm *msg_item2 = new RecentMessageListItemForm(this);
    item2->setSizeHint(msg_item2->sizeHint());
    ui->listWidget->addItem(item2);
    ui->listWidget->setItemWidget(item2, msg_item2);

    
}

RecentMessageListForm::~RecentMessageListForm()
{
    delete ui;
}
