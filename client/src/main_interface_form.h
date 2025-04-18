#ifndef MAIN_INTERFACE_FORM_H
#define MAIN_INTERFACE_FORM_H

#include <QWidget>
#include "global.h"
#include "sidebar_form.h"
#include "search_bar_form.h"
#include "recent_message_list_form.h"
#include "friend_list_form.h"
#include "chat_form.h"
#include "scroll_list_form.h"


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

private slots:
    void slot_goto_recent_message_list();
    void slot_goto_friend_list();
    void slot_goto_search_result_list();

private:
    Ui::MainInterfaceForm *ui;
    SidebarForm *sidebar_form_;
    SearchBarForm *search_bar_form_;
    RecentMessageListForm *recent_message_list_form_;
    FriendListForm *friend_list_form_;
    ChatForm *chat_form_;
    ScrollListForm *scroll_list_form_;
};

#endif // MAIN_INTERFACE_FORM_H
