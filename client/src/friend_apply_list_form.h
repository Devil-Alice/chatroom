#ifndef FRIEND_APPLY_LIST_FROM_H
#define FRIEND_APPLY_LIST_FROM_H

#include <QWidget>
#include "global.h"
#include "scroll_list_form.h"

namespace Ui {
class FriendApplyListForm;
}

class FriendApplyListForm : public ScrollListForm
{
    Q_OBJECT

public:
    explicit FriendApplyListForm(QWidget *parent = nullptr);
    ~FriendApplyListForm();
    void add_friend_apply_item_from_json(QJsonObject json_obj);
    void update_friend_apply_item_from_json(QJsonObject json_obj);
    int get_unhandled_apply_num();

signals:
    void signal_set_unhandled_apply_nums(int num);

public slots:
    void slot_send_friend_apply_finished(QJsonObject json_data);
    void slot_query_friend_apply_finished(QJsonObject json_data); 
    void slot_notify_receive_friend_apply(QJsonObject json_data);
    void slot_handle_apply_finished(QJsonObject json_data);


private:
    QVector<FriendApply> friend_applys_;
};

#endif // SCROLL_LIST_FORM_H
