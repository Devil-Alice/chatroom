#ifndef REGISTER_DIALOG_H
#define REGISTER_DIALOG_H

#include <QDialog>
#include <QTimer>
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

signals:
    void signal_return();

public slots:
    void slot_get_verify_code();
    void slot_register_user();
    void slot_register_request_finished(MY_STATUS_CODE code, REQUEST_ID request_id, QString response);
    void slot_check_name();
    void slot_check_phone();
    void slot_check_verify_code();
    void slot_check_password();
    void slot_check_confirm_password();

private:
    QMap<REQUEST_ID, std::function<void(QJsonObject &json_obj)>> http_response_handler_;
    int time_count_;
    QTimer *timer_;
    QMap<QString, QString> tips_;
    void init_response_handler();
    // status 有两种，“success”是绿色，“error”是红色
    void show_register_msg(QString msg, QString status);
    void show_tip();
    void add_tip(QString key, QString value);
    void remove_tip(QString key);

private:
    Ui::RegisterDialog *ui;
};

#endif // REGISTER_DIALOG_H
