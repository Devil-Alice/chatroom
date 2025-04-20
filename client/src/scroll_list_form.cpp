#include "scroll_list_form.h"
#include "ui_scroll_list_form.h"
#include "tcp_manager.h"
#include "friend_apply_item_form.h"

ScrollListForm::ScrollListForm(QWidget *parent) : QWidget(parent),
                                                  ui(new Ui::ScrollListForm)
{
    ui->setupUi(this);

    connect(this, &ScrollListForm::signal_build_list, this, &ScrollListForm::set_widgets);

    connect(TcpManager::instance().get(), &TcpManager::signal_search_content_finished, this, &ScrollListForm::slot_search_content_finished);
    connect(TcpManager::instance().get(), &TcpManager::signal_send_friend_apply_finished, this, &ScrollListForm::slot_send_friend_apply_finished);
    connect(TcpManager::instance().get(), &TcpManager::signal_query_friend_apply_finished, this, &ScrollListForm::slot_query_friend_apply_finished);
}

ScrollListForm::~ScrollListForm()
{
    delete ui;
}

void ScrollListForm::set_widgets(QVector<std::shared_ptr<QWidget>> widgets)
{
    clear_widgets();
    widgets_.append(widgets);

    for (int i = 0; i < widgets_.size(); i++)
    {
        ui->layout_list->addWidget(widgets_[i].get());
        widgets_[i]->show();
    }

    return;
}

void ScrollListForm::clear_widgets()
{
    widgets_.clear();

    // QLayoutItem *item = nullptr;
    // while ((item = ui->layout_list->takeAt(0)) != nullptr)
    // {
    //     if (item->widget())
    //         item->widget()->deleteLater();

    //     delete item;
    // }
}

void ScrollListForm::slot_search_content_finished(QJsonObject json_data)
{
    // 解析json
    int status = json_data["status"].toInt();
    QString msg = json_data["message"].toString();

    if (status != MY_STATUS_CODE::SUCCESS)
    {
        QMessageBox::information(this, "info", msg);
        return;
    }

    QJsonArray json_array = json_data["data"].toArray();
    // 构造展示的数据列表
    QVector<std::shared_ptr<QWidget>> widgets;
    for (int i = 0; i < json_array.size(); i++)
    {
        QJsonValueRef item = json_array[i];
        QJsonObject item_json = item.toObject();
        QString uid = item_json["uid"].toString();
        QString name = item_json["name"].toString();
        QString avatar = item_json["avatar"].toString();

        // 根据uid排除自己搜索信息
        if (uid != self_info->uid_)
        {
            std::shared_ptr<QWidget> item_form(new SearchedItemForm(uid, name, avatar));
            widgets.push_back(item_form);
        }
    }

    emit signal_build_list(widgets);

    return;
}

void ScrollListForm::slot_send_friend_apply_finished(QJsonObject json_data)
{
}

void ScrollListForm::slot_query_friend_apply_finished(QJsonObject json_data)
{
    // 解析json
    int reply_status = json_data["status"].toInt();
    QString msg = json_data["message"].toString();

    if (reply_status != MY_STATUS_CODE::SUCCESS)
    {
        QMessageBox::information(this, "info", msg);
        return;
    }

    QJsonArray json_array = json_data["data"].toArray();
    // 构造展示的数据列表
    QVector<std::shared_ptr<QWidget>> widgets;

    // 构造scroll list中的元素
    for (int i = 0; i < json_array.size(); i++)
    {
        auto item = json_array[i].toObject();

        QJsonObject from_user_json = item["from_user"].toObject();
        QJsonObject to_user_json = item["to_user"].toObject();

        QString from_uid = from_user_json["uid"].toString();
        QString from_name = from_user_json["name"].toString();
        QString from_avatar = from_user_json["avatar"].toString();
        User from_user(from_uid, from_name, from_avatar);

        QString to_uid = to_user_json["uid"].toString();
        QString to_name = to_user_json["name"].toString();
        QString to_avatar = to_user_json["avatar"].toString();
        User to_user(to_uid, to_name, to_avatar);

        QString remark_name = item["remark_name"].toString();
        QString apply_message = item["apply_message"].toString();
        int status = item["status"].toInt();
        FriendApply friend_apply(from_user, to_user, remark_name, apply_message, status);

        auto widget = std::make_shared<FriendApplyItemForm>(friend_apply);
        widgets.push_back(widget);
    }

    emit signal_build_list(widgets);
    return;
}


