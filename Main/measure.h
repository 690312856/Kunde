#ifndef MEASURE_H
#define MEASURE_H

// own
#include "communication.h"
#include "wait.h"

#include <QThread>
#include <algorithm>
using namespace std;

/*
 * 测量类
 * 多线程实现不阻塞UI完成测量。
 * 返回测试完成信号和处理好的电压信号
 */
class Measure : public QObject
{
    Q_OBJECT
public:
    Measure(QByteArray instruction);            // 构造函数，已指令为参数
    ~Measure();

    double getV();

signals:
    void sendV(double, double);               // 发送电压信号
    void receiveFinish();
    void workFinished();                        // 完成工作信号

public slots:
    void start();                               // 开始测量
    void work();                                // 正式测量
    void receiveWait(int);                      // 接收等待

private:
    QThread *thread;                            // 依赖线程
    QByteArray instruction;                     // 指令
};

#endif // MEASURE_H
