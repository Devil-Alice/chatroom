#include "main_window.h"
#include "ui_main_window.h"
#include "login_dialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    
    login_dialog = new LoginDialog(this);
    // 设置无边框
    login_dialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    resize(login_dialog->size());
    setCentralWidget(login_dialog);
    move_to_screen_center(this);

    // connect注册消息处理
    connect(login_dialog, &LoginDialog::signal_goto_register, this, &MainWindow::slot_goto_register);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_goto_login()
{
    // 创建登陆界面的实列
    login_dialog = new LoginDialog(this);
    // 设置无边框
    login_dialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    // connect注册消息处理
    connect(login_dialog, &LoginDialog::signal_goto_register, this, &MainWindow::slot_goto_register);

    setCentralWidget(login_dialog);
    register_dialog->hide();
    resize(login_dialog->size());
    move_to_screen_center(this);
    login_dialog->show();
}

void MainWindow::slot_goto_register()
{
    // 创建注册界面的实例
    register_dialog = new RegisterDialog(this);
    register_dialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    // 处理注册界面的返回信号
    connect(register_dialog, &RegisterDialog::signal_return, this, &MainWindow::slot_goto_login);

    setCentralWidget(register_dialog);
    login_dialog->hide();
    resize(register_dialog->size());
    move_to_screen_center(this);
    register_dialog->show();
}
