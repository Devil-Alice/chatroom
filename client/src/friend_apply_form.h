#ifndef FRIEND_APPLY_FORM_H
#define FRIEND_APPLY_FORM_H

#include <QWidget>
#include "global.h"

namespace Ui {
class FriendApplyForm;
}

class FriendApplyForm : public QWidget
{
    Q_OBJECT

public:
    explicit FriendApplyForm(QString to_uid, QString name, QWidget *parent = nullptr);
    ~FriendApplyForm();

private:
    void send_apply();
    void close_window();

private:
    Ui::FriendApplyForm *ui;
    QString to_uid_;
    QString name_;
};

#endif // FRIEND_APPLY_FORM_H
