#ifndef REGISTER_DIALOG_H
#define REGISTER_DIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

public slots:
    void slot_get_verify_code();
    void slot_register_request_finished(MY_STATUS_CODE code, REQUEST_ID request_id, QString response);

private:
    QMap<REQUEST_ID, std::function<void(QJsonObject &json_obj)>> http_request_handler_;
    void init_request_handler();
    // status 有两种，“success”是绿色，“error”是红色
    void show_register_msg(QString msg, QString status);

private:
    Ui::RegisterDialog *ui;
};

#endif // REGISTER_DIALOG_H
