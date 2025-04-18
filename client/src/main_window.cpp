#include "main_window.h"
#include "ui_main_window.h"
#include "login_dialog.h"

std::shared_ptr<SelfInfo> self_info = std::make_shared<SelfInfo>();

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    flag_enable_resize_ = true;

    this->menuBar()->hide();   // 隐藏菜单栏
    this->statusBar()->hide(); // 隐藏状态栏

    // 最开始显示登录
    show_login_dialog();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::show_login_dialog()
{
    enable_resize(true);
    // 创建登陆界面的实列
    login_dialog_ = new LoginDialog(this);
    // 设置无边框
    login_dialog_->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    // connect注册消息处理
    connect(login_dialog_, &LoginDialog::signal_goto_register, this, &MainWindow::slot_goto_register);
    connect(login_dialog_, &LoginDialog::signal_goto_main_interface, this, &MainWindow::slot_goto_main_interface);

    setCentralWidget(login_dialog_);
    resize(login_dialog_->size());
    move_to_screen_center(this);
    login_dialog_->show();
    // 开启登录界面后，禁止改变窗口大小
    enable_resize(false);
}

void MainWindow::show_register_dialog()
{
    enable_resize(true);
    // 创建注册界面的实例
    register_dialog_ = new RegisterDialog(this);
    register_dialog_->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    // 处理注册界面的返回信号
    connect(register_dialog_, &RegisterDialog::signal_return, this, &MainWindow::slot_goto_login);

    setCentralWidget(register_dialog_);
    resize(register_dialog_->size());
    move_to_screen_center(this);
    register_dialog_->show();
    enable_resize(false);
}

void MainWindow::show_main_interface()
{
    // 开启改变大小
    enable_resize(true);
    // 创建注册界面的实例
    main_interface_form_ = new MainInterfaceForm(this);
    main_interface_form_->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    setCentralWidget(main_interface_form_);
    resize(main_interface_form_->size());
    move_to_screen_center(this);
    main_interface_form_->show();
}

void MainWindow::enable_resize(bool flag)
{
    if (flag)
    {
        Qt::WindowFlags flags = windowFlags();
        flags &= ~Qt::MSWindowsFixedSizeDialogHint; // 移除这个 flag
        setWindowFlags(flags);
        show(); // 更新窗口样式
    }
    else
    {
       setWindowFlags(windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
       show();
    }
}

void MainWindow::slot_goto_login()
{
    show_login_dialog();
}

void MainWindow::slot_goto_register()
{
    show_register_dialog();
}

void MainWindow::slot_goto_main_interface()
{
    show_main_interface();
}
