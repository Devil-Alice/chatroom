#include "searched_list_form.h"
#include "tcp_manager.h"

SearchedListForm::SearchedListForm(QWidget *parent) : ScrollListForm(parent)
{
    connect(TcpManager::instance().get(), &TcpManager::signal_search_content_finished, this, &SearchedListForm::slot_search_content_finished);
}

SearchedListForm::~SearchedListForm()
{
}


void SearchedListForm::slot_search_content_finished(QJsonObject json_data)
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