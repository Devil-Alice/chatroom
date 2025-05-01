#ifndef FRIEND_APPLY_ITEM_FORM_H
#define FRIEND_APPLY_ITEM_FORM_H

#include <QWidget>
#include "global.h"

namespace Ui
{
    class FriendApplyItemForm;
}

class FriendApplyItemForm : public QWidget
{
    Q_OBJECT

    FriendApply friend_apply_;

public:
    explicit FriendApplyItemForm(FriendApply friend_apply, QWidget *parent = nullptr);
    ~FriendApplyItemForm();
    static std::shared_ptr<FriendApplyItemForm> parse_from_json(QJsonObject json_obj);
    FriendApply get_friend_apply();

public slots:
    void slot_btn_accpet_clicked();
    void slot_btn_reject_clicked();


private:
    Ui::FriendApplyItemForm *ui;
};

#endif // FRIEND_APPLY_ITEM_FORM_H
