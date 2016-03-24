#include "serialports.h"

const int BLOCK_SIZE = 5;
const QByteArray VERIFY_SEND(1, (char)0xAA);
const QByteArray VERIFY_RECEIVE(QByteArray(4, char(0xBB)) + QByteArray(1, char(0)));
const QByteArray RECEIVE_ERROR(QByteArray(4, char(0xFF)) + QByteArray(1, char(0)));

SerialPorts::SerialPorts(QString serialPortName) :
    QThread(NULL)
  , serialPortName(serialPortName)
  , connectTimes(0)
{
    serialPort = new QextSerialPort(serialPortName);
    connect(serialPort,SIGNAL(readyRead()),this,SLOT(receive()));

    serialPort->setBaudRate((BaudRateType)9600);
    serialPort->setDataBits((DataBitsType)8);
    serialPort->setParity(PAR_NONE);
    serialPort->setStopBits(STOP_1);
    serialPort->setFlowControl(FLOW_OFF);
    serialPort->setTimeout(10);

    if(!serialPort->open(QIODevice::ReadWrite))
    {
        serialPortStatus = SERIALPORTS_INVALID;
    }
    else
    {
        serialPortStatus = SERIALPORTS_WAIT;
        start(); // connect
    }
}

SerialPorts::~SerialPorts()
{
    closeSerialPort();
}

void SerialPorts::closeSerialPort()
{
    if(serialPort)
    {
        serialPort->close();
        delete serialPort;
        serialPort=NULL;
    }
}

void SerialPorts::run()
{
    sleep(2);
    send(VERIFY_SEND);
}

void SerialPorts::receive()
{
    // 叠加接收的数据
    receiveData += serialPort->readAll();

    // 是否传输完成
    if (receiveData.size() != BLOCK_SIZE) return;

    if (!dataCheck())
    {
        // 数据未通过校验，要求重发
        qDebug() << "check failed~";
        receiveData.clear();
        serialPort->write (QByteArray(1, char(0xDF)));
        return;
    }

    if (receiveData[0] == (char)0xFF || receiveData[2] == (char)0xFF)
    {
        qDebug() << "receiveFailed failed~";
        receiveData.clear();
        serialPort->write (QByteArray(1, char(0xD0)));
        serialPort->write (sendData);
        return;
    }

    // 处理
    analyzeReceive();
    serialPort->write (QByteArray(1, char(0xD0)));

    // 清空接收的数据
    receiveData.clear();
}

void SerialPorts::analyzeReceive()
{
    // 是否是连接认证通信
    if (sendData == VERIFY_SEND)
    {
        if (receiveData == VERIFY_RECEIVE)
        {
            serialPortStatus = SERIALPORTS_OK;
            emit verifyFinish(VERIFY_OK, this);
        }
        // 不是则重试
        else
        {
            connectTimes++;
            // 重试3次则为连接失败
            if(connectTimes==3)
            {
                emit verifyFinish(VERIFY_FAIL,this);
                qDebug() << str("失败3次，最后一次接收数据为：");
            }
            else
            {
                this->start();
            }
        }
    }
    // 不是连接认证通信则发送接收完毕信号
    else
    {
        QString tmp = "";
        char str[100];

        foreach(char c, receiveData)
        {
            sprintf(str, "%02X", (0xFF&(char)c));
            tmp += str;
            tmp += " ";
        }

        qDebug() << tmp;

        emit receiveFinish(sendData,receiveData);
    }
}

/*
 * 发送数据，返回true成功/false失败
 */
bool SerialPorts::send(QByteArray data)
{
    int ret = serialPort->write (sendData = data);
    return ret != -1;
}

bool SerialPorts::isAvailable()
{
    return serialPortStatus != SERIALPORTS_INVALID;
}

bool SerialPorts::dataCheck()
{
    char tmp = 0;
    foreach(char ch, receiveData) tmp ^= ch;
    return tmp == 0;         // 返回校验结果，返回0说明校验成功
}
