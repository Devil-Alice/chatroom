#ifndef LOGIN_DIALOG_H
#define LOGIN_DIALOG_H

#include <QDialog>
#include "global.h"
#include "http_manager.h"
#include "tcp_manager.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private:
    Ui::LoginDialog *ui;
    
    QMap<REQUEST_ID, std::function<void(QJsonObject&)>> http_response_handler_;
    void init_response_handler();
signals:
    void signal_goto_register();
    void signal_connect_to_server(ConnectoinInfo info);
    void signal_goto_main_interface();
public slots:
    void slot_user_login();
    void slot_login_request_finished(MY_STATUS_CODE code, REQUEST_ID request_id, QString response);
    void slot_handle_connection_status(bool success);
    void slot_chat_login_finished(QJsonObject json_data);
};

#endif // LOGIN_DIALOG_H
