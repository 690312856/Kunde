#ifndef CONTROLER_H
#define CONTROLER_H

#include "measure.h"
#include "measurestatus.h"
#include "widget.h"
#include "database.h"

#include "comcommunication.h"
#include "calculatemethod.h"

#include <QPushButton>

/*
 * Controler类，控制
 * 对界面，测量进行控制
 */
class Widget;
class Controler : public QObject
{
    Q_OBJECT
public:
    enum CommunicationStatus{COMMUNICATION_OK, COMMUNICATION_FAIL, COMMUNICATION_WAIT};
    enum CombinationMethod{SingleCombination, DoubleCombination};

public:
    Controler(QObject *parent = 0);
    ~Controler();

    void doMeasure(CombinationMethod = DoubleCombination);
    void init();

    void checkFinish();
    void writeToDatabase();

signals:

public slots:
    void startGroup();
    void startMeasure();
    void doMeasureAgain();
    void startRemeasure();

private:
    Widget widget;
    CalculateMethod calculateMethod;
    MeasureStatus measureStatus;
};

#endif // CONTROLER_H
