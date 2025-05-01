#include "scroll_list_form.h"
#include "ui_scroll_list_form.h"
#include "tcp_manager.h"
#include "friend_apply_item_form.h"

ScrollListForm::ScrollListForm(QWidget *parent) : QWidget(parent),
                                                  ui(new Ui::ScrollListForm)
{
    ui->setupUi(this);

    connect(this, &ScrollListForm::signal_build_list, this, &ScrollListForm::show_widgets);   
}

ScrollListForm::~ScrollListForm()
{
    delete ui;
}

void ScrollListForm::show_widgets(QVector<std::shared_ptr<QWidget>> widgets)
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

