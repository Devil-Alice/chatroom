#ifndef RECENT_MESSAGE_LIST_FORM_H
#define RECENT_MESSAGE_LIST_FORM_H

#include <QWidget>

namespace Ui {
class RecentMessageListForm;
}

class RecentMessageListForm : public QWidget
{
    Q_OBJECT

public:
    explicit RecentMessageListForm(QWidget *parent = nullptr);
    ~RecentMessageListForm();

private:
    Ui::RecentMessageListForm *ui;
};

#endif // RECENT_MESSAGE_LIST_FORM_H
