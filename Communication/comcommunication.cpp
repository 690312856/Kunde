#include "comcommunication.h"

ComCommunication::ComCommunication(QObject *parent) :
    Communication(parent)
{
    portNum = 0;
    estimatedNum = 0;
    verifyFinishNum = 0;

    // 等待连接类发送的信号，最长6.5s.连接最大时间是6s.
    Wait wait(this, SIGNAL(connectFinish()), 300000);
    this->tryConnect();
    wait.start();
    qDebug() << (wait.isTimeout()?str("认证超时"):str("正常"));
}

Communication * ComCommunication::instance()
{
    if(_instance == 0)
        _instance = new ComCommunication;
    return _instance;
}

void ComCommunication::tryConnect()
{
    for(int i=0;i<15;i++)
    {
        SerialPorts* tempPort = new SerialPorts(QString("COM%0").arg(i));
        if(tempPort->isAvailable())
        {
            estimatedNum++;
            connect(tempPort,SIGNAL(verifyFinish(SerialPorts::VerifyStatus,SerialPorts*)),
                    this,SLOT(establishConnection(SerialPorts::VerifyStatus,SerialPorts*)));
        }
        else
        {
            delete tempPort;
        }
    }

    // 可以确定没有设备
    if (estimatedNum == 0)
    {
        deviceStatus = NO_DEVICE;
        qDebug() << str("认证失败");
        emit connectFinish();
    }
}

void ComCommunication::establishConnection(SerialPorts::VerifyStatus verifyStatus, SerialPorts* vPort)
{
    qDebug() << "connection~";
    verifyFinishNum++;
    if (verifyStatus == SerialPorts::VERIFY_OK)
    {
        port[portNum++] = vPort;
    }

    // 当估计的串口都测试完时
    if (verifyFinishNum == estimatedNum)
    {
        if(portNum)
        {
            connect(port[0],SIGNAL(receiveFinish(QByteArray,QByteArray)),
                    this,SLOT(receive(QByteArray,QByteArray)));
            deviceStatus = portNum == 1 ? ONE_DEVICE : MORE_THAN_ONE_DEVICE;
            qDebug() << str("连接成功");
        }
        else
        {
            deviceStatus = NO_DEVICE;
            qDebug() << str("连接失败");
        }

        emit connectFinish();
    }
}

bool ComCommunication::send(QByteArray sendData)
{
    if(portNum)
        return port[0]->send(sendData);
    qDebug() << str("没有可用串口");
    return false;
}

ComCommunication::~ComCommunication()
{
    if( portNum )
    {
        for(int i=0;i<portNum;i++)
            delete port[i];
    }
}
