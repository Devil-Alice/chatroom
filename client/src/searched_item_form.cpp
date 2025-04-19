#include "searched_item_form.h"
#include "ui_searched_item_form.h"
#include "tcp_manager.h"
#include "friend_apply_form.h"

SearchedItemForm::SearchedItemForm(QString uid, QString name, QString avatar, QWidget *parent) : QWidget(parent),
                                                                                    ui(new Ui::SearchedItemForm)
{
    ui->setupUi(this);

    uid_ = uid;
    name_ = name;
    avatar_ = avatar;

    ui->label_avatar->setFixedSize(40, 40);
    ui->label_avatar->setScaledContents(true);
    if (avatar_.isEmpty())
        avatar_ = ":/image/user.png";
    ui->label_avatar->setPixmap({avatar_});

    ui->label_name->setText(name_);

    ui->pushButton->setText("添加");


    connect(ui->pushButton, &QPushButton::clicked, this, &SearchedItemForm::slot_goto_friend_apply_form);
}

SearchedItemForm::~SearchedItemForm()
{
    delete ui;
}

void SearchedItemForm::slot_goto_friend_apply_form()
{
    friend_apply_form_ = QSharedPointer<FriendApplyForm>::create(uid_, name_);
    friend_apply_form_->setWindowFlag(Qt::Window);
    friend_apply_form_->show();
}

