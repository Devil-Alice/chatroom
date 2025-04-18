#include "search_bar_form.h"
#include "ui_search_bar_form.h"

SearchBarForm::SearchBarForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchBarForm)
{
    ui->setupUi(this);


    connect(ui->btn_search, &QPushButton::clicked, this, &SearchBarForm::slot_btn_search_clicked);
}

SearchBarForm::~SearchBarForm()
{
    delete ui;
}

void SearchBarForm::slot_btn_search_clicked()
{
    // 发送查询请求：
    QString search_text = ui->text_search->text();
    if (search_text.isEmpty())
    {
        return;
    }

    // 构造请求数据
    QJsonObject json_obj;

    json_obj["uid"] = self_info->uid_;
    json_obj["token"] = self_info->token_;
    json_obj["search_content"] = search_text;

    QJsonDocument json_doc(json_obj);

    emit TcpManager::instance()->signal_send_message(REQUEST_ID::SEARCH_CONTENT, json_doc.toJson());

    emit signal_goto_search_result_list();
}
