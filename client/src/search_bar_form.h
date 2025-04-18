#ifndef SEARCH_BAR_FORM_H
#define SEARCH_BAR_FORM_H

#include <QWidget>
#include "global.h"
#include "tcp_manager.h"

namespace Ui {
class SearchBarForm;
}

class SearchBarForm : public QWidget
{
    Q_OBJECT

public:
    explicit SearchBarForm(QWidget *parent = nullptr);
    ~SearchBarForm();

signals:
    void signal_goto_search_result_list();

public slots:
    void slot_btn_search_clicked();

private:
    Ui::SearchBarForm *ui;
};

#endif // SEARCH_BAR_FORM_H
