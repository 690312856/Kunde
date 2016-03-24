#include "measure.h"

Measure::Measure(QByteArray instruction) :
    QObject()
  , instruction(instruction)
{

    thread = new QThread();
    this->moveToThread(thread);

    connect(thread, SIGNAL(started()), this, SLOT(work()));
    connect(this, SIGNAL(workFinished()), thread, SLOT(quit()));
    connect(thread, SIGNAL(finished()), this, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    connect(Communication::instance(), SIGNAL(receiveSize(int)), this, SLOT(receiveWait(int)));
}

Measure::~Measure()
{
    if(thread->isRunning())
        thread->terminate();
}

void Measure::work()
{
    qDebug() << "measure~";

    bool success = false;
    int  times = 0;
    do
    {
        Wait wait(this, SIGNAL(receiveFinish()), 100000);
    //    showByteArray(instruction);
        Communication::instance()->send(instruction);
        wait.start();
        if(wait.isTimeout()) times++;
        else success =true;
    } while (!success && times<3);

    if (times>=3)
    {
        qDebug() << str("测量3次失败！");
    }
    else
    {
        double v1 = getV();
        double v2 = getV();

        double maxV = max(v1, v2);
        double minV = min(v1, v2);

        // 10%修正
        if(maxV*0.90>=minV)
        {
            work();
            return;
        }
        else
        {
            emit sendV(v1, v2);
        }
    }

    emit workFinished();
    thread->quit();
}

void Measure::receiveWait(int size)
{
    if (size == 2)
        emit receiveFinish();
}

void Measure::start()
{
    if (!thread->isRunning())
        thread->start();
}

double Measure::getV()
{
    QByteArray receive = Communication::instance()->getReceive();
//    showByteArray(receive);

    double tmp = 0;

    tmp = (receive[1]&0xF0)/16*100.0 + (receive[1]&0x0F)*10.0
        + (receive[2]&0xF0)/16*1.0   + (receive[2]&0x0F)/10.0
        + (receive[3]&0xF0)/16/100.0;
    qDebug() << tmp;

    return tmp;
}
