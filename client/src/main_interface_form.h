#ifndef MAIN_INTERFACE_FORM_H
#define MAIN_INTERFACE_FORM_H

#include <QWidget>
#include "global.h"
#include "friend_list_form.h"
#include "chat_form.h"

namespace Ui {
class MainInterfaceForm;
}

class MainInterfaceForm : public QWidget
{
    Q_OBJECT

public:
    explicit MainInterfaceForm(QWidget *parent = nullptr);
    ~MainInterfaceForm();

private:
    Ui::MainInterfaceForm *ui;
    
    FriendListForm *friend_list_form_;
    ChatListForm *chat_list_from_;
};

#endif // MAIN_INTERFACE_FORM_H
