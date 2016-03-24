#ifndef COMCOMMUNICATION_H
#define COMCOMMUNICATION_H

// own
#include "communication.h"
#include "wait.h"
#include "serialports.h"
#include "com_global.h"

class COM_SHARED_EXPORT ComCommunication : public Communication
{
    Q_OBJECT
protected:
    ComCommunication(QObject *parent = 0);

public:
    ~ComCommunication();

    static Communication * instance();

    void tryConnect();
    bool send(QByteArray);

public slots:
    // 建立连接
    void establishConnection(SerialPorts::VerifyStatus, SerialPorts*);

private:
    int          estimatedNum;
    int          verifyFinishNum;
    int          portNum;
    SerialPorts* port[15];
};

#endif // COMCOMMUNICATION_H
