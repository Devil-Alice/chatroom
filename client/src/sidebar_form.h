#ifndef SIDEBAR_FORM_H
#define SIDEBAR_FORM_H

#include <QWidget>
#include "global.h" 

namespace Ui {
class SidebarForm;
}

class SidebarForm : public QWidget
{
    Q_OBJECT

public:
    explicit SidebarForm(QWidget *parent = nullptr);
    ~SidebarForm();
    void set_message_notify_visible(bool is_visible);
    void set_friend_notify_visible(bool is_visible);

signals:
    void signal_goto_friend_list();
    void signal_goto_recent_message_list();

private:
    Ui::SidebarForm *ui;
};

#endif // SIDEBAR_FORM_H
