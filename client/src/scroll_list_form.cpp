#include "scroll_list_form.h"
#include "ui_scroll_list_form.h"
#include "tcp_manager.h"

ScrollListForm::ScrollListForm(QWidget *parent) : QWidget(parent),
                                                  ui(new Ui::ScrollListForm)
{
    ui->setupUi(this);

    connect(this, &ScrollListForm::signal_build_list, this, &ScrollListForm::set_widgets);

    connect(TcpManager::instance().get(), &TcpManager::signal_search_content_finished, this, &ScrollListForm::slot_search_content_finished);
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
