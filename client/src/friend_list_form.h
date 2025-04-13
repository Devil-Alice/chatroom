#ifndef FRIEND_LIST_FORM_H
#define FRIEND_LIST_FORM_H

#include <QWidget>
#include "global.h"

namespace Ui {
class FriendListForm;
}

class FriendListForm : public QWidget
{
    Q_OBJECT

public:
    explicit FriendListForm(QWidget *parent = nullptr);
    ~FriendListForm();

private:
    Ui::FriendListForm *ui;
};

#endif // FRIEND_LIST_FORM_H
