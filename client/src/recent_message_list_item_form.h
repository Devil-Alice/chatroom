#ifndef RECENT_MESSAGE_LIST_ITEM_FORM_H
#define RECENT_MESSAGE_LIST_ITEM_FORM_H

#include <QWidget>

namespace Ui {
class RecentMessageListItemForm;
}

class RecentMessageListItemForm : public QWidget
{
    Q_OBJECT

public:
    explicit RecentMessageListItemForm(QWidget *parent = nullptr);
    ~RecentMessageListItemForm();

private:
    Ui::RecentMessageListItemForm *ui;
};

#endif // RECENT_MESSAGE_LIST_ITEM_FORM_H
