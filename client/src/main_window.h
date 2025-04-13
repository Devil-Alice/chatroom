#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_main_window.h"
#include "login_dialog.h"
#include "register_dialog.h"
#include "main_interface_form.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private:
    void show_login_dialog();
    void show_register_dialog();
    void show_main_interface();
    void enable_resize(bool flag);

public slots:
    void slot_goto_register();
    void slot_goto_login();
    void slot_goto_main_interface();

private:
    Ui::MainWindow *ui;
    LoginDialog *login_dialog_;
    RegisterDialog *register_dialog_;
    MainInterfaceForm *main_interface_form_;
    bool flag_enable_resize_;

};
#endif // MAINWINDOW_H
