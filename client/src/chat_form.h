#ifndef CHAT_FORM_H
#define CHAT_FORM_H

#include <QWidget>
#include "global.h"
#include "chat_message_item_form.h"

namespace Ui {
class ChatForm;
}

class ChatForm : public QWidget
{
    Q_OBJECT

public:
    explicit ChatForm(QWidget *parent = nullptr);
    ~ChatForm();

private:
    void add_message(int role);

private:
    Ui::ChatForm *ui;
};

#endif // CHAT_FORM_H
