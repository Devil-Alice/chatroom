#ifndef MAIN_INTERFACE_FORM_H
#define MAIN_INTERFACE_FORM_H

#include <QWidget>
#include "global.h"
#include "sidebar_form.h"
#include "recent_message_list_form.h"
#include "friend_list_form.h"
#include "chat_form.h"

namespace Ui
{
    class MainInterfaceForm;
}

class MainInterfaceForm : public QWidget
{
    Q_OBJECT

public:
    explicit MainInterfaceForm(QWidget *parent = nullptr);
    ~MainInterfaceForm();

private:
    void show_recent_message_list();
    void show_friend_list();

private:
    Ui::MainInterfaceForm *ui;
    SidebarForm *sidebar_form_;
    RecentMessageListForm *recent_message_list_form_;
    FriendListForm *friend_list_form_;
    ChatForm *chat_list_from_;
};

#endif // MAIN_INTERFACE_FORM_H
