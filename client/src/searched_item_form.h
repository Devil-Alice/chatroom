#ifndef SEARCHED_ITEM_FORM_H
#define SEARCHED_ITEM_FORM_H

#include <QWidget>
#include "global.h"
#include "friend_apply_form.h"

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
public slots:
    void slot_goto_friend_apply_form();


private:
    Ui::SearchedItemForm *ui;
    QString uid_;
    QString name_;
    QString avatar_;
    QSharedPointer<FriendApplyForm> friend_apply_form_;
};

#endif // SEARCHED_ITEM_FORM_H
