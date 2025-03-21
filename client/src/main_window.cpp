#include "main_window.h"
#include "ui_main_window.h"
#include "login_dialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    

    // 创建登陆界面的实列
    login_dialog = new LoginDialog(this);
    // 设置无边框
    login_dialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setFixedSize(login_dialog->size());
    setCentralWidget(login_dialog);

    // connect注册消息处理
    connect(login_dialog, &LoginDialog::signal_goto_register, this, &MainWindow::slot_goto_register);


    // 创建注册界面的实例
    register_dialog = new RegisterDialog(this);
    register_dialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_goto_register()
{
    setCentralWidget(register_dialog);
    login_dialog->hide();
    setFixedSize(register_dialog->size());
    register_dialog->show();
}
