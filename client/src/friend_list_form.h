#ifndef FRIEND_LIST_FORM_H
#define FRIEND_LIST_FORM_H

#include <QWidget>
#include "global.h"
#include "friend_list_item_form.h"

namespace Ui {
class FriendListForm;
}

class FriendListForm : public QWidget
{
    Q_OBJECT

public:
    explicit FriendListForm(QWidget *parent = nullptr);
    ~FriendListForm();

public slots:
    void slot_item_clicked(QListWidgetItem *item);


private:
    Ui::FriendListForm *ui;
};

#endif // FRIEND_LIST_FORM_H
