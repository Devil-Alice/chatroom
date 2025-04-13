#ifndef CHAT_FORM_H
#define CHAT_FORM_H

#include <QWidget>

namespace Ui {
class ChatListForm;
}

class ChatListForm : public QWidget
{
    Q_OBJECT

public:
    explicit ChatListForm(QWidget *parent = nullptr);
    ~ChatListForm();

private:
    Ui::ChatListForm *ui;
};

#endif // CHAT_FORM_H
