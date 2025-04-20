#include "friend_list_form.h"
#include "ui_friend_list_form.h"
#include "tcp_manager.h"

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
    connect(ui->btn_friend_apply, &QPushButton::clicked, this, &FriendListForm::slot_btn_friend_apply_clicked);
    connect(TcpManager::instance().get(), &TcpManager::signal_handle_friend_apply_finished, this, &FriendListForm::slot_handle_friend_apply_finished);

}

FriendListForm::~FriendListForm()
{
    delete ui;
}

void FriendListForm::slot_btn_friend_apply_clicked()
{
    // 向服务器获取好友申请
    QJsonObject json_obj = self_info->get_basic_info_json();
    QJsonDocument json_doc(json_obj);

    emit TcpManager::instance()->signal_send_message(REQUEST_ID::QUERY_FRIEND_APPLY, json_doc.toJson());

    emit slot_goto_friend_apply_list();
}

void FriendListForm::slot_item_clicked(QListWidgetItem *item)
{
    FriendListItemForm *friend_list_item = qobject_cast<FriendListItemForm*>(ui->listWidget->itemWidget(item));
        
}

void FriendListForm::slot_handle_friend_apply_finished(QJsonObject json_data)
{
    // 解析json
    int reply_status = json_data["status"].toInt();
    QString msg = json_data["message"].toString();

    if (reply_status != MY_STATUS_CODE::SUCCESS)
    {
        QMessageBox::information(this, "info", msg);
        return;
    }

    // 重新查询一次
    slot_btn_friend_apply_clicked();
    return;
}