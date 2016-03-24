#ifndef MEASURESTATUS_H
#define MEASURESTATUS_H

#include "widget.h"
#include <QObject>

class Controler;
typedef QMap<QString, double> DataMap;
class MeasureStatus : public QObject
{
    Q_OBJECT
    friend class Controler;
public:
    MeasureStatus(QObject *parent = 0);

    void clearData();
    void setPointIndex(QString);
    void setParameterMap(const QStringMap &value);

    int  getCurrentPoint();
    bool isFinishGroup();

    void clearPreRow();
    void clearAllRow();

signals:
    void finishOnePoint(int, double);
    void finishOneRow();

public slots:
    void getOneRowData(DataMap);

private:
    int         pointNum;
    int         currentPoint;
    int         currentRow;
    QList<int>  points;

    QStringMap parameterMap;
};

#endif // MEASURESTATUS_H
