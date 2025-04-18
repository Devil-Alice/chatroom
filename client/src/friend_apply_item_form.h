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

private:
    Ui::FriendApplyItemForm *ui;
};

#endif // FRIEND_APPLY_ITEM_FORM_H
