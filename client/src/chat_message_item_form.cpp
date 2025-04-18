#include "chat_message_item_form.h"
#include "ui_chat_message_item_form.h"
#include <QScrollBar>

int ChatMessageItemForm::max_message_width_ = 0;

ChatMessageItemForm::ChatMessageItemForm(int role, QWidget *parent) : QWidget(parent),
                                                                      ui(new Ui::ChatMessageItemForm)
{
    ui->setupUi(this);

    // 设置消息的一些属性
    flag_width_set_ = false;
    ui->text_message->setReadOnly(true);
    ui->text_message->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->text_message->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->text_message->document()->setDocumentMargin(0);

    margins[0] = 0;
    margins[1] = 0;
    margins[2] = 0;
    margins[3] = 0;

    if (role == 0)
    {
        ui->label_self_avatar->hide();
        ui->label_name->setAlignment(Qt::AlignLeft);
        ui->horizontalLayout_message->setAlignment(Qt::AlignLeft);
    }
    else if (role == 1)
    {
        ui->label_user_avatar->hide();
        ui->label_name->setAlignment(Qt::AlignRight);
        ui->horizontalLayout_message->setAlignment(Qt::AlignRight);
    }

    ui->text_message->setText("test");
}

ChatMessageItemForm::~ChatMessageItemForm()
{
    delete ui;
}

void ChatMessageItemForm::set_width()
{

    QTextDocument *doc = ui->text_message->document();
    QFontMetricsF fm(ui->text_message->font());

    int line_rect_width = ui->text_message->document()->size().width() * 0.8;
    // 更新最大宽度
    if (max_message_width_ == 0)
        max_message_width_ = line_rect_width;

    int max_line_text_width = 0;
    for (QTextBlock it = doc->begin(); it != doc->end(); it = it.next())
    {
        int line_text_width = fm.width(it.text());
        max_line_text_width = qMax(max_line_text_width, line_text_width);
        qDebug() << "line_text_width: " << line_text_width;
    }

    ui->text_message->getContentsMargins(&margins[0], &margins[1], &margins[2], &margins[3]);
    // qDebug() << "left" << margins[0];
    // qDebug() << "top" << margins[1];
    // qDebug() << "right" << margins[2];
    // qDebug() << "bottom" << margins[3];
    // qDebug() << "padding" << ui->text_message->document()->documentMargin();

    int doc_margin = ui->text_message->document()->documentMargin() * 2 + margins[0] + margins[2];
    ui->text_message->setMaximumWidth(qMin(max_line_text_width, max_message_width_) + doc_margin + 2);
}

void ChatMessageItemForm::paintEvent(QPaintEvent *evnet)
{
    if (flag_width_set_ == false)
    {
        set_width();
        flag_width_set_ = true;
        qDebug() << "test" << endl;
    }

    // 这里似乎需要加上2才是真正的高度，否则会出现滚动条，原因未知
    int doc_height = ui->text_message->document()->size().height();
    ui->text_message->setFixedHeight(doc_height + margins[1] + margins[3] + 2);
}
