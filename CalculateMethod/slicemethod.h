#ifndef SLICEMETHOD_H
#define SLICEMETHOD_H

#include "cal_global.h"
#include "readdata.h"
#include <QObject>

typedef QMap<QString, double> DataMap;
typedef QMap<QString, QString> QStringMap;

class CAL_SHARED_EXPORT SliceMethod : public QObject
{
public:
    static double formulaCalculate(QStringMap &parameterMap, QList<double> &v);
    static double resultsCorrected(QStringMap& parameterMap, QList<double>& v);
    static double temperatureCorrected(QStringMap& parameterMap, double p0);
    static void calculate(QStringMap &parameterMap, QList<double> &v, DataMap& data);
};

#endif // SLICEMETHOD_H
