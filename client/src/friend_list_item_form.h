#ifndef FRIEND_LIST_ITEM_FORM_H
#define FRIEND_LIST_ITEM_FORM_H

#include <QWidget>

namespace Ui {
class FriendListItemForm;
}

class FriendListItemForm : public QWidget
{
    Q_OBJECT

public:
    explicit FriendListItemForm(QWidget *parent = nullptr);
    ~FriendListItemForm();

private:
    Ui::FriendListItemForm *ui;
};

#endif // FRIEND_LIST_ITEM_FORM_H
