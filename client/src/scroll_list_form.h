#ifndef SCROLL_LIST_FORM_H
#define SCROLL_LIST_FORM_H

#include <QWidget>
#include "searched_item_form.h"
#include "global.h"

namespace Ui {
class ScrollListForm;
}

class ScrollListForm : public QWidget
{
    Q_OBJECT

public:
    explicit ScrollListForm(QWidget *parent = nullptr);
    ~ScrollListForm();
    void set_widgets(QVector<std::shared_ptr<QWidget>> widgets);
    void clear_widgets();

signals:
    void signal_build_list( QVector<std::shared_ptr<QWidget>> widgets_);

public slots:
    void slot_search_content_finished(QJsonObject json_data);
    void slot_query_friend_apply_finished(QJsonObject json_data);

private:
    Ui::ScrollListForm *ui;
    QVector<std::shared_ptr<QWidget>> widgets_;
};

#endif // SCROLL_LIST_FORM_H
