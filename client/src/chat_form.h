#ifndef CHAT_FORM_H
#define CHAT_FORM_H

#include <QWidget>

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
    Ui::ChatForm *ui;
};

#endif // CHAT_FORM_H
