#include "friend_apply_list_form.h"
#include "tcp_manager.h"
#include "friend_apply_item_form.h"

FriendApplyListForm::FriendApplyListForm(QWidget *parent) : ScrollListForm(parent)
{
    connect(TcpManager::instance().get(), &TcpManager::signal_send_friend_apply_finished, this, &FriendApplyListForm::slot_send_friend_apply_finished);
    connect(TcpManager::instance().get(), &TcpManager::signal_query_friend_apply_finished, this, &FriendApplyListForm::slot_query_friend_apply_finished);
}

FriendApplyListForm::~FriendApplyListForm()
{
}

void FriendApplyListForm::add_friend_apply_item_from_json(QJsonObject json_obj)
{
    // 解析json
    int reply_status = json_obj["status"].toInt();
    QString msg = json_obj["message"].toString();

    if (reply_status != MY_STATUS_CODE::SUCCESS)
    {
        QMessageBox::information(this, "info", msg);
        return;
    }

    auto item = json_obj["data"].toObject();

    auto widget = FriendApplyItemForm::parse_from_json(item);
    auto apply = widget->get_friend_apply();
    for (int i = 0 ; i < friend_applys_.size(); i++)
    {
        auto apply_tmp = friend_applys_[i];
        if (apply.from_user_.uid_ == apply_tmp.from_user_.uid_ 
            && apply.to_user_.uid_ == apply_tmp.to_user_.uid_)
            {
                // 找到了相同请求，更新
                widgets_[i] = widget;
                show_widgets(widgets_);
                return;
            }
    }

    // 没找到相同的请求，追加
    widgets_.push_back(widget);
    show_widgets(widgets_);
    
}

void FriendApplyListForm::slot_send_friend_apply_finished(QJsonObject json_data)
{
}

void FriendApplyListForm::slot_query_friend_apply_finished(QJsonObject json_data)
{
    // 一次查询清空friend applys
    friend_applys_.clear();

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

        auto widget = FriendApplyItemForm::parse_from_json(item);
        widgets.push_back(widget);

        // 保存friend_apply
        friend_applys_.push_back(widget->get_friend_apply());
    }

    show_widgets(widgets);
    return;
}
