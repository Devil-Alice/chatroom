#include "chat_form.h"
#include "ui_chat_form.h"

ChatForm::ChatForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatForm)
{
    ui->setupUi(this);

    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 0);

    
}

ChatForm::~ChatForm()
{
    delete ui;
}
