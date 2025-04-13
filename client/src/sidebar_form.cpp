#include "sidebar_form.h"
#include "ui_sidebar_form.h"

SidebarForm::SidebarForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SidebarForm)
{
    ui->setupUi(this);
    
    // setFixedWidth(60);


}

SidebarForm::~SidebarForm()
{
    delete ui;
}
