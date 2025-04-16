#ifndef CHAT_MESSAGE_ITEM_FORM_H
#define CHAT_MESSAGE_ITEM_FORM_H

#include <QWidget>
#include "global.h"

namespace Ui
{
    class ChatMessageItemForm;
}

class ChatMessageItemForm : public QWidget
{
    Q_OBJECT

public:
    // role 为1表示自己，0表示其他用户
    explicit ChatMessageItemForm(int role, QWidget *parent = nullptr);
    ~ChatMessageItemForm();

protected:
    void set_width();
    void paintEvent(QPaintEvent *evnet) override;

private:
    Ui::ChatMessageItemForm *ui;
    bool flag_width_set_;
    static int max_message_width_;
    int margins[4];

};

#endif // CHAT_MESSAGE_ITEM_FORM_H
