#include "login_dialog.h"
#include "ui_login_dialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    ui->text_password->setEchoMode(QLineEdit::Password);

    // 当点击click时，发送signal_goto_register信号
    connect(ui->btn_register, &QPushButton::clicked, this, &LoginDialog::signal_goto_register);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}
