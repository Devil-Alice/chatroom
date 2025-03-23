#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_main_window.h"
#include "login_dialog.h"
#include "register_dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void slot_goto_register();
    void slot_goto_login();

private:
    Ui::MainWindow *ui;
    LoginDialog *login_dialog;
    RegisterDialog *register_dialog;
};
#endif // MAINWINDOW_H
