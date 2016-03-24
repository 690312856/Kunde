#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "com_global.h"

#include <QObject>
#include <QQueue>
#include <QDebug>
/*
 * 通信类。完成与单片机间的通信。
 * 提供receive信号。得到信号后可通过getReceive()获取接受到的数据
 */
class COM_SHARED_EXPORT Communication : public QObject
{
    Q_OBJECT
public:
    enum DeviceStatus {LOADING, NO_DEVICE, ONE_DEVICE, MORE_THAN_ONE_DEVICE};

protected:
    Communication(QObject *parent = 0);         // 通信对象唯一限定

public:
    ~Communication();                           // 析构函数
    static Communication * instance();          // 唯一实例
    static void deleteInstance();               // 删除实例

protected:
    static Communication * _instance;           // 唯一实例

public:
    virtual void tryConnect() = 0;              // 试图连接
    virtual bool send(QByteArray) = 0;          // 发送数据

    QByteArray getReceive();                    // 获取接收到的数据
    DeviceStatus getConnectionStatus();         // 获取当前的连接状态

signals:
    void connectFinish();                       // 连接完成信号
    void receiveFinish();                       // 接收完成信号
    void receiveSize(int);                      // 当前接收数据队列大小

public slots:
    void receive(QByteArray, QByteArray);       // 接收数据

protected:
    DeviceStatus deviceStatus;                  // 连接状态

    QByteArray sendData;                        // 发送数据
    QQueue<QByteArray> receiveDataQueue;        // 接收数据队列

    QMutex mutex;
};

#endif // COMMUNICATION_H
