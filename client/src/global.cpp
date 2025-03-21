#include "global.h"

QString gate_url_prefix = "";

void refresh_style(QWidget *widget)
{
    widget->style()->unpolish(widget);
    widget->style()->polish(widget);
}