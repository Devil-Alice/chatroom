#include "main_window.h"

#include <QApplication>
#include <QFile>
#include <QDebug> // 添加 qDebug() 进行调试输出

int main(int argc, char *argv[])
{
            
    // 启用自动 DPI 缩放
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // 启用高 DPI 缩放
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);   // 启用高 DPI 图片支持
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
            qDebug() << "qss file load failed";
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
