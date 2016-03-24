#ifndef CALCULATEMETHOD_H
#define CALCULATEMETHOD_H

#include <QObject>
#include <QDebug>

#include "cal_global.h"
#include "readdata.h"
#include "slicemethod.h"

typedef QMap<QString, double> DataMap;
typedef QMap<QString, QString> QStringMap;

class CAL_SHARED_EXPORT CalculateMethod : public QObject
{
    Q_OBJECT
public:
    enum Method {SLICE_METHOD};

public:
    CalculateMethod(QObject *parent = 0);

    void setParameter(QStringMap &);
    void setCurrentPoint(int);
    void calculate();
    QStringList getCurrentHeader();

    static void soundTip();

signals:
    void sendOneRowData(DataMap);

public slots:
    void getV(double, double);
    void setCurrentMethod(const Method &value);

protected:
    QList<double> v;
    QStringMap parameterMap;

    Method currentMethod;
};

#endif // CALCULATEMETHOD_H
