#include "main_window.h"

#include <QApplication>
#include <QFile>
#include <QDebug> // 添加 qDebug() 进行调试输出

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // try
    // {
        // 加载qss样式
        QFile qss_file(":/style/styles.css");
        if (qss_file.open(QFile::ReadOnly))
        {
            QString style_sheet = qss_file.readAll();
            a.setStyleSheet(style_sheet);
            qss_file.close();
        }
        else
        {
            qDebug() << "qss 文件加载失败！请检查资源文件路径是否正确。";
        }

        gate_url_prefix = "http://" + (*app_config)["server"]["host"] + ":" + (*app_config)["server"]["port"];

        // 启动主窗口
        MainWindow w;
        w.show();
    // }
    // catch (const std::exception &e)
    // {
    //     qDebug() << e.what() << '\n';
    // }

    return a.exec();
}
