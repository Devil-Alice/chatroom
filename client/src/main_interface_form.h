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

private:
    // 隐藏layout中所有的窗体
    void hide_all_widgets_in_layout(QLayout *layout);

private slots:
    void slot_goto_recent_message_list();
    void slot_goto_friend_list();
    void slot_goto_scroll_list();

private:
    Ui::MainInterfaceForm *ui;
    SidebarForm *sidebar_form_;
    SearchBarForm *search_bar_form_;
    RecentMessageListForm *recent_message_list_form_;
    FriendListForm *friend_list_form_;
    ChatForm *chat_form_;
    // scroll_list_form_是一个通用的滚动列表，可以承载好友查询结果、好友申请信息等
    ScrollListForm *scroll_list_form_;
};

#endif // MAIN_INTERFACE_FORM_H
