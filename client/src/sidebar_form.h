#ifndef SIDEBAR_FORM_H
#define SIDEBAR_FORM_H

#include <QWidget>

namespace Ui {
class SidebarForm;
}

class SidebarForm : public QWidget
{
    Q_OBJECT

public:
    explicit SidebarForm(QWidget *parent = nullptr);
    ~SidebarForm();

signals:
    void signal_goto_friend_list();
    void signal_goto_recent_message_list();

private:
    Ui::SidebarForm *ui;
};

#endif // SIDEBAR_FORM_H
