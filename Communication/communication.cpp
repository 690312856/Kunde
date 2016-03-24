#include "communication.h"

Communication * Communication::_instance = 0;

Communication::Communication(QObject *parent) :
    QObject(parent)
  , deviceStatus(LOADING)
{
}

Communication::~Communication()
{
    _instance = 0;
}

// 该类的唯一实例，保证通信设备的单一，方便访问
Communication * Communication::instance()
{
    return _instance;
}

void Communication::deleteInstance()
{
    if(_instance != 0)
        delete _instance;
    _instance = 0;
}

Communication::DeviceStatus Communication::getConnectionStatus()
{
    return deviceStatus;
}

void Communication::receive(QByteArray sendData, QByteArray receiveData)
{
    this->receiveDataQueue.push_back(receiveData);
    emit receiveFinish();
    emit receiveSize(receiveDataQueue.size());
}

QByteArray Communication::getReceive()
{
    if (receiveDataQueue.isEmpty())
    {
        qDebug() << "Receive pool no data~";
        return QByteArray();
    }

    QByteArray tmp(receiveDataQueue.first());
    receiveDataQueue.pop_front();
    return tmp;
}
