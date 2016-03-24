#ifndef SERIALPORTS_H
#define SERIALPORTS_H

// own
#include "qextserial/qextserialport.h"
#include "com_global.h"

#include <QThread>
#include <QDebug>
/*
 *  串口端口类
 */
class SerialPorts : public QThread
{
    Q_OBJECT
public:
    enum SerialPortStatus   {SERIALPORTS_INVALID, SERIALPORTS_OK, SERIALPORTS_WAIT};
    enum VerifyStatus       {VERIFY_OK,VERIFY_FAIL};

    // 构造函数，串口名为参数
    SerialPorts(QString serialPortName);
    // 析构函数
    ~SerialPorts();

    // 关闭串口端口
    void closeSerialPort();

    // 发送数据
    bool send(QByteArray data);
    // 处理接收数据
    void analyzeReceive();
    // 数据校验
    bool dataCheck();

    // 串口是否可用
    bool isAvailable();

signals:
    // 认证结束信号
    void verifyFinish(SerialPorts::VerifyStatus, SerialPorts *);
    // 接收结束信号
    void receiveFinish(QByteArray, QByteArray);

public slots:
    // 接收槽
    void receive();

protected:
    // 多线程，延时发送数据
    void run();

private:
    QString             serialPortName;
    QextSerialPort*     serialPort;

    QByteArray          sendData;
    QByteArray          receiveData;

    SerialPortStatus    serialPortStatus;
    int                 connectTimes;
};

#endif // SERIALPORTS_H
