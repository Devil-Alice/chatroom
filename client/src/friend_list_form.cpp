#include "friend_list_form.h"
#include "ui_friend_list_form.h"

FriendListForm::FriendListForm(QWidget *parent) : QWidget(parent),
                                                  ui(new Ui::FriendListForm)
{
    ui->setupUi(this);

    setMinimumWidth(250);

    // 测试数据
    QListWidgetItem *item = new QListWidgetItem();
    FriendListItemForm *friend_item = new FriendListItemForm(this);
    item->setSizeHint(friend_item->sizeHint());
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, friend_item);


    QListWidgetItem *item2 = new QListWidgetItem();
    FriendListItemForm *friend_item2 = new FriendListItemForm(this);
    item2->setSizeHint(friend_item2->sizeHint());
    ui->listWidget->addItem(item2);
    ui->listWidget->setItemWidget(item2, friend_item2);

    connect(ui->listWidget, &QListWidget::itemClicked, this,  &FriendListForm::slot_item_clicked);


}

FriendListForm::~FriendListForm()
{
    delete ui;
}

void FriendListForm::slot_item_clicked(QListWidgetItem *item)
{
    FriendListItemForm *friend_list_item = qobject_cast<FriendListItemForm*>(ui->listWidget->itemWidget(item));
        
}
