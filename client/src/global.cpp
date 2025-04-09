#include "global.h"

QString gate_url_prefix = "";

void refresh_style(QWidget *widget)
{
    widget->style()->unpolish(widget);
    widget->style()->polish(widget);
}

QString md5_encrypt(const QString &data)
{
    // md5加密后，是十六进制字，转换为字符串是需要注意
    QByteArray encrypted_data_hex = QCryptographicHash::hash(data.toUtf8(), QCryptographicHash::Md5);
    return encrypted_data_hex.toHex();
}


void move_to_screen_center(QWidget *window)
{
    // 获取屏幕的中心点
    QDesktopWidget *desktop = QApplication::desktop();
    QRect screenGeometry = desktop->screenGeometry();
    int screenCenterX = screenGeometry.center().x();
    int screenCenterY = screenGeometry.center().y();

    // 计算窗口的新位置，使其中心对齐屏幕中心
    int windowCenterX = window->width() / 2;
    int windowCenterY = window->height() / 2;
    int newX = screenCenterX - windowCenterX;
    int newY = screenCenterY - windowCenterY;

    // 设置窗口位置
    window->move(newX, newY);
}