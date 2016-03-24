#ifndef READDATA_H
#define READDATA_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#define str(a) QString::fromLocal8Bit(a)

#include "cal_global.h"

class CAL_SHARED_EXPORT ReadData : public QObject
{
    Q_OBJECT
public:
    typedef QMap<double, double> DataMap;

    static void readData();
    static void readCorrectedData(DataMap &data, QString fileName);

    static double getCorrectedData(DataMap&, double);
    static double thicknessCorrect(double);
    static double positionCorrect(double);
    static double temperatureNCorrect(double);
    static double temperaturePCorrect(double);

private:
    static DataMap thicknessData;
    static DataMap positionData;
    static DataMap temperatureNData;
    static DataMap temperaturePData;
};

#endif // READDATA_H
