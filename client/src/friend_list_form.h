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

signals:
    void slot_goto_friend_apply_list();

public slots:
    void slot_item_clicked(QListWidgetItem *item);
    void slot_btn_friend_apply_clicked();
    void slot_handle_friend_apply_finished(QJsonObject json_data);
    

private:
    Ui::FriendListForm *ui;
};

#endif // FRIEND_LIST_FORM_H
