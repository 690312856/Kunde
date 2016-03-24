/*
 * Four Probe Measure SoftWare
 * Author : SF-_-
 * Email  : 1930795729@qq.com
 * If you hava any question, or you need help, you can contant me :)
 */
#include "controler.h"
#include <QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<double>("resType");
    qRegisterMetaType<DataMap>("DataMap");

    // 读取文本文件中的数据
    ReadData::readData();

    QSplashScreen splash(QPixmap(str("://image/昆德启动画面.png")));
    splash.setDisabled(true);
    splash.show();
    splash.setFont(QFont(str("微软雅黑"),12));
    splash.showMessage(str("连接串口中……"),
                       Qt::AlignTop | Qt::AlignRight, Qt::white);

    ComCommunication::instance();               // create communication connection.
                                                // judge the connection status.
    if (Communication::instance()->getConnectionStatus()
                                 == Communication::NO_DEVICE)
    {
        splash.showMessage(str("无法连接到设备！\n请确定已将设备接入计算机中"),
                           Qt::AlignCenter | Qt::AlignVCenter, Qt::white);
    }

    Controler c;
    splash.finish(Widget::instance());
    Widget::instance()->show();

    return a.exec();
}
