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
    void query_friend_applys();
    // 展示未处理的好友请求数量，0则不显示
    void set_unhandled_friend_apply_num(int num);

signals:
    void slot_goto_friend_apply_list();

public slots:
    void slot_item_clicked(QListWidgetItem *item);
    void slot_btn_friend_apply_clicked();

private:
    Ui::FriendListForm *ui;
    QLabel *label_unhandled_num;
};

#endif // FRIEND_LIST_FORM_H
