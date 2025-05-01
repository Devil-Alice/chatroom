#ifndef SEARCHED_LIST_FORM_H
#define SEARCHED_LIST_FORM_H

#include <QWidget>
#include "global.h"
#include "scroll_list_form.h"

namespace Ui {
class SearchedListForm;
}

class SearchedListForm : public ScrollListForm
{
    Q_OBJECT

public:
    explicit SearchedListForm(QWidget *parent = nullptr);
    ~SearchedListForm();


public slots:
    void slot_search_content_finished(QJsonObject json_data);


};

#endif // SCROLL_LIST_FORM_H
