#include "wait.h"

Wait::Wait(QObject *sender, const char *signal, int deadlineMsec, int interval)
{
    timeIsUp = false;

    timer.setInterval(interval);
    connect(sender, signal, &timer, SLOT(start()));
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));

    timeout.setInterval(deadlineMsec);
    timeout.setSingleShot(true);
    connect(&timeout, SIGNAL(timeout()), this, SLOT(deadLineTimeout()));
    connect(&timeout, SIGNAL(timeout()), &loop, SLOT(quit()));
}

Wait::~Wait()
{
    timer.stop();
    loop.quit();
}

void Wait::start()
{
    timeout.start();
    loop.exec();
    timeout.stop();
}

void Wait::deadLineTimeout()
{
    timeIsUp = true;
}

bool Wait::isTimeout()
{
    return timeIsUp;
}

void Wait::sleep(int msec)
{
    // 使用QTimer和QEventLoop，完成sleep功能。
    QTimer timer;
    QEventLoop loop;
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    timer.start(msec);
    loop.exec();
    timer.stop();
}
