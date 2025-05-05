#include "friend_list_form.h"
#include "ui_friend_list_form.h"
#include "tcp_manager.h"

FriendListForm::FriendListForm(QWidget *parent) : QWidget(parent),
                                                  ui(new Ui::FriendListForm)
{
    ui->setupUi(this);

    setMinimumWidth(250);

    label_unhandled_num = new QLabel(ui->btn_friend_apply);
    label_unhandled_num->setText("");
    label_unhandled_num->hide();
    label_unhandled_num->setStyleSheet((R"(
            background-color: red;
            color: white;
            border-radius: 5px;
            padding: 1px;)"));

    set_unhandled_friend_apply_num(0);

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

    connect(ui->listWidget, &QListWidget::itemClicked, this, &FriendListForm::slot_item_clicked);
    connect(ui->btn_friend_apply, &QPushButton::clicked, this, &FriendListForm::slot_btn_friend_apply_clicked);
}

FriendListForm::~FriendListForm()
{
    delete ui;
}

void FriendListForm::query_friend_applys()
{
    // 向服务器获取好友申请
    QJsonObject json_obj = self_info->get_basic_info_json();
    QJsonDocument json_doc(json_obj);

    emit TcpManager::instance() -> signal_send_message(REQUEST_ID::QUERY_FRIEND_APPLY, json_doc.toJson());

    emit slot_goto_friend_apply_list();
}

void FriendListForm::set_unhandled_friend_apply_num(int num)
{
    if (num == 0)
    {
        label_unhandled_num->hide();
        return;
    }
    QString num_str = QString::number(num);
    label_unhandled_num->setText(num_str);
    label_unhandled_num->adjustSize();

    // 计算label的位置，实现右对其的效果
    auto label_size = label_unhandled_num->size();
    auto btn_size = ui->btn_friend_apply->size();

    int x = btn_size.width() - label_size.width() - 5;
    int y = btn_size.height() / 2 - label_size.height() / 2;

    label_unhandled_num->move(x, y);

    label_unhandled_num->show();
    return;
}

void FriendListForm::slot_btn_friend_apply_clicked()
{
    query_friend_applys();
}

void FriendListForm::slot_item_clicked(QListWidgetItem *item)
{
    FriendListItemForm *friend_list_item = qobject_cast<FriendListItemForm *>(ui->listWidget->itemWidget(item));
}
