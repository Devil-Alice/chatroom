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
