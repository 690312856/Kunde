#ifndef POINTPLAN_H
#define POINTPLAN_H

#include <QWidget>
#include <QDialog>
#include <QPainter>
#include <QMouseEvent>
#include <cmath>
#include <QDebug>
#include <algorithm>
using namespace std;

typedef QMap<int, double> Data;
typedef QMap<int, double>::iterator It;

class PointPlan : public QDialog
{
    Q_OBJECT
    enum PointPlans{PlanA, PlanB, PlanC, PlanD, PlanE, PlanF, PlanG, PlanH};

public:
    PointPlan(QWidget *parent = 0);
    PointPlan(PointPlan * cmp, QWidget *parent = 0);

    int getCurrentPoint() const;
    void setCurrentPoint(int value);

    QList<int> getCenterPoint();

    void addData(int x, double value);
    void clearData();

    PointPlans getCurrentPlan() const;
    void setCurrentPlan(const PointPlans &value);

public slots:
    void setCurrentPlan(const QString &value);

protected:
    void paintEvent(QPaintEvent *);

    void paintPlanA();
    void paintPlanB();
    void paintPlanC();
    void paintPlanD();
    void paintPlanE();
    void paintPlanF();
    void paintPlanG();
    void paintPlanH();

private:
    Data data;

    int currentPoint;
    PointPlans currentPlan;
};

#endif // POINTPLAN_H
