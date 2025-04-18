#ifndef SEARCHED_ITEM_FORM_H
#define SEARCHED_ITEM_FORM_H

#include <QWidget>
#include "global.h"

namespace Ui
{
    class SearchedItemForm;
}

class SearchedItemForm : public QWidget
{
    Q_OBJECT

public:
    explicit SearchedItemForm(QString uid, QString name, QString avatar, QWidget *parent = nullptr);
    ~SearchedItemForm();
    void slot_send_friend_apply();

private:
    Ui::SearchedItemForm *ui;
    QString uid_;
    QString name_;
    QString avatar_;
};

#endif // SEARCHED_ITEM_FORM_H
