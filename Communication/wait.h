#ifndef WAIT_H
#define WAIT_H

#include "com_global.h"
#include <QObject>
#include <QTimer>
#include <QEventLoop>

class COM_SHARED_EXPORT Wait : public QObject
{
    Q_OBJECT
public:
    Wait(QObject * sender, const char* signal, int deadlineMsec = 1000, int interval = 100);
    ~Wait();

    void start();
    bool isTimeout();

    static void sleep(int msec);

public slots:
    void deadLineTimeout();

private:
    QEventLoop loop;
    QTimer timer;
    QTimer timeout;

    bool timeIsUp;
};

#endif // WAIT_H
