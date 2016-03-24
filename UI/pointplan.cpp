#include "pointplan.h"
#include<iostream>

PointPlan::PointPlan(QWidget *parent)
    : QDialog(parent)
    , currentPlan(PlanA)
    , currentPoint(0)
{
    setMouseTracking(true);
    setAutoFillBackground(true);
    QPalette p = this->palette();
    p.setColor(QPalette::Window,Qt::white);
    setPalette(p);
    resize(250, 250);
}

PointPlan::PointPlan(PointPlan *cmp, QWidget *parent)
    : QDialog(parent)
    , currentPlan(cmp->currentPlan)
    , currentPoint(cmp->currentPoint)
    , data(cmp->data)
{
    qDebug() << data;
    setMouseTracking(true);
    setAutoFillBackground(true);
    QPalette p = this->palette();
    p.setColor(QPalette::Window,Qt::white);
    setPalette(p);
    resize(600, 600);
    update();
}

void PointPlan::paintEvent(QPaintEvent *)
{
    switch(currentPlan)
    {
    case PlanA: paintPlanA();break;
    case PlanB: paintPlanB();break;
    case PlanC: paintPlanC();break;
    case PlanD: paintPlanD();break;
    case PlanE: paintPlanE();break;
    case PlanF: paintPlanF();break;
    case PlanG: paintPlanG();break;
    case PlanH: paintPlanH();break;
    }
}

void PointPlan::paintPlanA()//五点测量
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int width = this->width();
    int height = this->height();
    int shortEdge = min(width, height);


    QPointF points[6];
    // 圆心
    points[0+1] = QPointF(width/2.0, height/2.0);
    // 半径
    double r = (shortEdge - 10)/2.0;

    points[1+1] = points[0+1]+QPointF(r/2, r/2*sqrt(3.0))/2;
    points[3+1] = points[0+1]-QPointF(r/2, r/2*sqrt(3.0))/2;
    points[2+1] = points[0+1]+QPointF(r/2*sqrt(3.0), -r/2)/2;
    points[4+1] = points[0+1]-QPointF(r/2*sqrt(3.0), -r/2)/2;

    // 外围圆
    painter.setBrush(Qt::transparent);
    painter.setPen(QPen(QBrush(Qt::black), 1.8, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawEllipse(QRectF(points[0+1]-QPointF(r,r),QSize(2*r, 2*r)));
    painter.setPen(QPen(QBrush(Qt::blue), 1.8, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(points[0+1]+QPointF(r/2.4, sqrt(r*r-r*r/2.4/2.4)), points[0+1]+QPointF(-r/2.4, sqrt(r*r-r*r/2.4/2.4)));
    // 内部圆
    painter.setPen(QPen(QBrush(Qt::black), 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawEllipse(QRectF(points[0+1]-QPointF(r/2,r/2),QSize(r, r)));
    painter.drawLine(points[0+1]+QPointF(0, r), points[0+1]-QPointF(0, r));

    // 30度线
    painter.setPen(QPen(QBrush(Qt::black), 1.4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(points[1+1]*2-points[0+1], points[3+1]*2-points[0+1]);
    painter.drawLine(points[2+1]*2-points[0+1], points[4+1]*2-points[0+1]);

    //1，2，3，4，5点
    painter.setFont(QFont(QString::fromLocal8Bit("黑体"), r/150*16));
    QPointF adjustPoint(-r/20,-r/9-10);
    QPointF adjustPoint2(r/20,-10);
    for(int i=1;i<=5;i++)
    {
        if (i == currentPoint)
        {
            painter.setPen(QPen(Qt::red));
            painter.setBrush(Qt::red);
        }
        else
        {
            painter.setPen(QPen(Qt::blue));
            painter.setBrush(Qt::black);
        }
        painter.drawEllipse(points[i], 3, 3);
        painter.drawText(QRectF(points[i]+adjustPoint, points[i]+adjustPoint2),
                         Qt::AlignCenter, QString("%0").arg(i));
    }

    painter.setFont(QFont(QString::fromLocal8Bit("黑体"), r/120*12));
    painter.setBrush(QBrush(QColor(0, 255, 255, 150)));

    for(It it=data.begin();it!=data.end();it++)
    {
        char str[100];
        sprintf_s(str, "%.3lf", it.value());
        painter.setPen(Qt::white);
        painter.drawRoundRect(QRect((points[it.key()]+QPointF(-r/5,4)).toPoint(), QSize(r/120*50,r/120*15)), 15, 15);
        painter.setPen(Qt::blue);
        painter.drawText(QRect((points[it.key()]+QPointF(-r/5,4)).toPoint(), QSize(r/120*50,r/120*15)), Qt::AlignCenter, str);
    }
}

void PointPlan::paintPlanB()//小面积六点测量
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int width = this->width();
    int height = this->height();
    int shortEdge = min(width, height);

    QPointF points[7];
    // 圆心
    points[1+1] = QPointF(width/2.0, height/2.0);
    // 半径
    double r = (shortEdge - 10)/2.0;

    points[2+1] = points[1+1]+QPointF(r/2, r/2*sqrt(3.0))/2;
    points[0+1] = points[1+1]-QPointF(r/2, r/2*sqrt(3.0))/2;
    points[4+1] = QPointF(width/2.0, height/2.0);
    points[5+1] = points[1+1]+QPointF(r/2*sqrt(3.0), -r/2)/2;
    points[3+1] = points[1+1]-QPointF(r/2*sqrt(3.0), -r/2)/2;

    // 外围圆
    painter.setBrush(Qt::transparent);
    painter.setPen(QPen(QBrush(Qt::black), 1.8, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawEllipse(QRectF(points[1+1]-QPointF(r,r),QSize(2*r, 2*r)));
    painter.setPen(QPen(QBrush(Qt::blue), 1.8, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(points[1+1]+QPointF(r/2.4, sqrt(r*r-r*r/2.4/2.4)), points[1+1]+QPointF(-r/2.4, sqrt(r*r-r*r/2.4/2.4)));

    // 内部圆
    painter.setPen(QPen(QBrush(Qt::black), 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawEllipse(QRectF(points[1+1]-QPointF(r/2,r/2),QSize(r, r)));
    painter.drawLine(points[1+1]+QPointF(0, r), points[1+1]-QPointF(0, r));

    // 30度线
    painter.setPen(QPen(QBrush(Qt::black), 1.4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(points[0+1]*2-points[1+1], points[2+1]*2-points[1+1]);
    painter.drawLine(points[3+1]*2-points[1+1], points[5+1]*2-points[1+1]);

    //1，2，3，4，5，6点
    painter.setFont(QFont(QString::fromLocal8Bit("黑体"), r/150*16));
    QPointF adjustPoint(0,-5);
    for(int i=1;i<=6;i++)
    {
        if (i == currentPoint)
        {
            painter.setPen(QPen(Qt::red));
            painter.setBrush(Qt::red);
        }
        else
        {
            painter.setPen(QPen(Qt::black));
            painter.setBrush(Qt::black);
        }
        if(i==5)
        {
            painter.drawEllipse(points[i], 3, 3);
            painter.drawText(points[i]+QPointF(r/100,-r/20), QString("%0").arg(i));
        }
        else if(i==2)
        {
            painter.drawEllipse(points[i], 3, 3);
            painter.drawText(points[i]+QPointF(-r/16,r/7), QString("%0").arg(i));
        }
        else
        {
            painter.drawEllipse(points[i], 3, 3);
            painter.drawText(points[i]+adjustPoint, QString("%0").arg(i));
        }
    }

    painter.setFont(QFont(QString::fromLocal8Bit("黑体"), r/120*12));
    painter.setBrush(QBrush(QColor(0, 255, 255, 150)));

    for(It it=data.begin();it!=data.end();it++)
    {
        char str[100];
        sprintf_s(str, "%.3lf", it.value());
        if(it.key()==5)
        {
            painter.setPen(Qt::white);
            painter.drawRoundRect(QRect((points[it.key()]+QPointF(-r/5,-r*140/500)).toPoint(), QSize(r/120*50,r/120*15)), 15, 15);
            painter.setPen(Qt::blue);
            painter.drawText(QRect((points[it.key()]+QPointF(-r/5,-r*140/500)).toPoint(), QSize(r/120*50,r/120*15)), Qt::AlignCenter, str);


        }
        else if(it.key()==2)
        {
            painter.setPen(Qt::white);
            painter.drawRoundRect(QRect((points[it.key()]+QPointF(-r/5,r*11/70)).toPoint(), QSize(r/120*50,r/120*15)), 15, 15);
            painter.setPen(Qt::blue);
            painter.drawText(QRect((points[it.key()]+QPointF(-r/5,r*11/70)).toPoint(), QSize(r/120*50,r/120*15)), Qt::AlignCenter, str);
        }
        else
        {
            painter.setPen(Qt::white);
            painter.drawRoundRect(QRect((points[it.key()]+QPointF(-r/5,4)).toPoint(), QSize(r/120*50,r/120*15)), 15, 15);
            painter.setPen(Qt::blue);
            painter.drawText(QRect((points[it.key()]+QPointF(-r/5,4)).toPoint(), QSize(r/120*50,r/120*15)), Qt::AlignCenter, str);
        }
    }
}
void PointPlan::paintPlanC()//十点测量法
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int width = this->width();
    int height = this->height();
    int shortEdge = min(width, height);

    QPointF points[11];
    // 圆心
    points[2+1] = QPointF(width/2.0, height/2.0);
    // 半径
    double r = (shortEdge - 10)/2.0;

    points[3+1] = points[2+1]+QPointF(r/2, r/2*sqrt(3.0))/2;
    points[1+1] = points[2+1]-QPointF(r/2, r/2*sqrt(3.0))/2;
    points[0+1] = points[2+1]-QPointF((r-20),(r-20)*sqrt(3.0))/2;
    points[4+1] = points[2+1]+QPointF((r-20),(r-20)*sqrt(3.0))/2;
    points[7+1] = QPointF(width/2.0, height/2.0);
    points[9+1] = points[2+1]+QPointF((r-20)*sqrt(3.0),-(r-20))/2;
    points[5+1] = points[2+1]-QPointF((r-20)*sqrt(3.0),-(r-20))/2;
    points[8+1] = points[2+1]+QPointF(r/2*sqrt(3.0), -r/2)/2;
    points[6+1] = points[2+1]-QPointF(r/2*sqrt(3.0), -r/2)/2;

    // 外围圆
    painter.setBrush(Qt::transparent);
    painter.setPen(QPen(QBrush(Qt::black), 1.8, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawEllipse(QRectF(points[2+1]-QPointF(r,r),QSize(2*r, 2*r)));
    painter.setPen(QPen(QBrush(Qt::blue), 1.8, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(points[2+1]+QPointF(r/2.4, sqrt(r*r-r*r/2.4/2.4)), points[2+1]+QPointF(-r/2.4, sqrt(r*r-r*r/2.4/2.4)));

    //边缘6mm圆
    painter.setPen(QPen(QBrush(Qt::black), 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawEllipse(QRectF(points[2+1]-QPointF(r-20,r-20),QSize(2*r-40,2*r-40)));
    painter.drawLine(points[2+1]+QPointF(0, r-20), points[2+1]-QPointF(0, r-20));

    // 内部圆
    painter.setPen(QPen(QBrush(Qt::black), 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawEllipse(QRectF(points[2+1]-QPointF(r/2,r/2),QSize(r, r)));
    painter.drawLine(points[2+1]+QPointF(0, r), points[2+1]-QPointF(0, r));

    // 30度线
    painter.setPen(QPen(QBrush(Qt::black), 1.4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(points[1+1]*2-points[2+1], points[3+1]*2-points[2+1]);
    painter.drawLine(points[6+1]*2-points[2+1], points[8+1]*2-points[2+1]);

    //1，2，3，4，5，6，7，8，9，10点
    painter.setFont(QFont(QString::fromLocal8Bit("黑体"), r/150*16));
    QPointF adjustPoint(-r/30,-5);
    for(int i=1;i<=10;i++)
    {
        if (i == currentPoint)
        {
            painter.setPen(QPen(Qt::red));
            painter.setBrush(Qt::red);
        }
        else
        {
            painter.setPen(QPen(Qt::black));
            painter.setBrush(Qt::black);
        }
        if(i==8)
        {
            painter.drawEllipse(points[i], 3, 3);
            painter.drawText(points[i]+QPointF(r/100,-r/20), QString("%0").arg(i));
        }
        else if(i==3)
        {
            painter.drawEllipse(points[i], 3, 3);
            painter.drawText(points[i]+QPointF(-r/14,r/7), QString("%0").arg(i));
        }
        else
        {
            painter.drawEllipse(points[i], 3, 3);
            painter.drawText(points[i]+adjustPoint, QString("%0").arg(i));
        }
    }

    painter.setFont(QFont(QString::fromLocal8Bit("黑体"), r/120*12));
    painter.setBrush(QBrush(QColor(0, 255, 255, 150)));

    for(It it=data.begin();it!=data.end();it++)
    {
        char str[100];
        sprintf_s(str, "%.3lf", it.value());
        if(it.key()==8)
        {
            painter.setPen(Qt::white);
            painter.drawRoundRect(QRect((points[it.key()]+QPointF(-r/5,-r*135/500)).toPoint(), QSize(r/120*50,r/120*15)), 15, 15);
            painter.setPen(Qt::blue);
            painter.drawText(QRect((points[it.key()]+QPointF(-r/5,-r*135/500)).toPoint(), QSize(r/120*50,r/120*15)), Qt::AlignCenter, str);
        }
        else if(it.key()==3)
        {
            painter.setPen(Qt::white);
            painter.drawRoundRect(QRect((points[it.key()]+QPointF(-r/5,r/7)).toPoint(), QSize(r/120*50,r/120*15)), 15, 15);
            painter.setPen(Qt::blue);
            painter.drawText(QRect((points[it.key()]+QPointF(-r/5,r/7)).toPoint(), QSize(r/120*50,r/120*15)), Qt::AlignCenter, str);
        }
        else
        {
            painter.setPen(Qt::white);
            painter.drawRoundRect(QRect((points[it.key()]+QPointF(-r/5,4)).toPoint(), QSize(r/120*50,r/120*15)), 15, 15);
            painter.setPen(Qt::blue);
            painter.drawText(QRect((points[it.key()]+QPointF(-r/5,4)).toPoint(), QSize(r/120*50,r/120*15)), Qt::AlignCenter, str);
        }
    }
}

void PointPlan::paintPlanD()//大面积六点测量法
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int width = this->width();
    int height = this->height();
    int shortEdge = min(width, height);

    QPointF points[7];
    // 圆心
    points[1+1] = QPointF(width/2.0, height/2.0);
    // 半径
    double r = (shortEdge - 10)/2.0;

    points[2+1] = points[1+1]+QPointF((r*4/5), (r*4/5)*sqrt(3.0))/2;
    points[0+1] = points[1+1]-QPointF((r*4/5), (r*4/5)*sqrt(3.0))/2;
    points[4+1] = QPointF(width/2.0, height/2.0);
    points[5+1] = points[1+1]+QPointF((r*4/5)*sqrt(3.0), -(r*4/5))/2;
    points[3+1] = points[1+1]-QPointF((r*4/5)*sqrt(3.0), -(r*4/5))/2;

    // 外围圆
    painter.setBrush(Qt::transparent);
    painter.setPen(QPen(QBrush(Qt::black), 1.8, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawEllipse(QRectF(points[1+1]-QPointF(r,r),QSize(2*r, 2*r)));
    painter.setPen(QPen(QBrush(Qt::blue), 1.8, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(points[1+1]+QPointF(r/2.4, sqrt(r*r-r*r/2.4/2.4)), points[1+1]+QPointF(-r/2.4, sqrt(r*r-r*r/2.4/2.4)));

    //边缘6mm圆
    painter.setPen(QPen(QBrush(Qt::black), 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawEllipse(QRectF(points[1+1]-QPointF(4*r/5,4*r/5),QSize(8*r/5,8*r/5)));
    painter.drawLine(points[1+1]+QPointF(0, 4*r/5), points[1+1]-QPointF(0, 4*r/5));

    // 30度线
    painter.setPen(QPen(QBrush(Qt::black), 1.4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(points[1+1]-QPointF((r/2),(r/2)*sqrt(3.0)),points[1+1]+QPointF((r/2),(r/2)*sqrt(3.0)));
    painter.drawLine(points[1+1]-QPointF((r/2)*sqrt(3.0),-(r/2)),points[1+1]+QPointF((r/2)*sqrt(3.0),-(r/2)));

    //1，2，3，4，5，6点
    painter.setFont(QFont(QString::fromLocal8Bit("黑体"), r/150*16));
    QPointF adjustPoint(-r/30,-5);
    for(int i=1;i<=6;i++)
    {
        if (i == currentPoint)
        {
            painter.setPen(QPen(Qt::red));
            painter.setBrush(Qt::red);
        }
        else
        {
            painter.setPen(QPen(Qt::black));
            painter.setBrush(Qt::black);
        }
        if(i==5)
        {
            painter.drawEllipse(points[i], 3, 3);
            painter.drawText(points[i]+QPointF(r/100,-r/20), QString("%0").arg(i));
        }
        else if(i==2)
        {
            painter.drawEllipse(points[i], 3, 3);
            painter.drawText(points[i]+QPointF(-r/14,r/7), QString("%0").arg(i));
        }
        else
        {
            painter.drawEllipse(points[i], 3, 3);
            painter.drawText(points[i]+adjustPoint, QString("%0").arg(i));
        }
    }

    painter.setFont(QFont(QString::fromLocal8Bit("黑体"), r/120*12));
    painter.setBrush(QBrush(QColor(0, 255, 255, 150)));

    for(It it=data.begin();it!=data.end();it++)
    {
        char str[100];
        sprintf_s(str, "%.3lf", it.value());
        if(it.key()==5)
        {
            painter.setPen(Qt::white);
            painter.drawRoundRect(QRect((points[it.key()]+QPointF(-r/5,-r*140/500)).toPoint(), QSize(r/120*50,r/120*15)), 15, 15);
            painter.setPen(Qt::blue);
            painter.drawText(QRect((points[it.key()]+QPointF(-r/5,-r*140/500)).toPoint(), QSize(r/120*50,r/120*15)), Qt::AlignCenter, str);


        }
        else if(it.key()==2)
        {
            painter.setPen(Qt::white);
            painter.drawRoundRect(QRect((points[it.key()]+QPointF(-r/5,r*11/70)).toPoint(), QSize(r/120*50,r/120*15)), 15, 15);
            painter.setPen(Qt::blue);
            painter.drawText(QRect((points[it.key()]+QPointF(-r/5,r*11/70)).toPoint(), QSize(r/120*50,r/120*15)), Qt::AlignCenter, str);
        }
        else
        {
            painter.setPen(Qt::white);
            painter.drawRoundRect(QRect((points[it.key()]+QPointF(-r/5,4)).toPoint(), QSize(r/120*50,r/120*15)), 15, 15);
            painter.setPen(Qt::blue);
            painter.drawText(QRect((points[it.key()]+QPointF(-r/5,4)).toPoint(), QSize(r/120*50,r/120*15)), Qt::AlignCenter, str);
        }
    }
}

void PointPlan::paintPlanE()//密集测量法
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int width = this->width();
    int height = this->height();
    int shortEdge = min(width, height);

    QPointF points[10];
    // 圆心
    points[4+1] = QPointF(width/2.0, height/2.0);
    // 半径
    double r = (shortEdge - 10)/2.0;

    points[3+1] = points[4+1]-QPointF((r/5), (r/5)*sqrt(3.0))/2;
    points[2+1] = points[4+1]-QPointF((r*2/5), (r*2/5)*sqrt(3.0))/2;
    points[1+1] = points[4+1]-QPointF((r*3/5), (r*3/5)*sqrt(3.0))/2;
    points[0+1] = points[4+1]-QPointF((r*4/5), (r*4/5)*sqrt(3.0))/2;
    points[8+1] = points[4+1]+QPointF((r*4/5), (r*4/5)*sqrt(3.0))/2;
    points[7+1] = points[4+1]+QPointF((r*3/5), (r*3/5)*sqrt(3.0))/2;
    points[6+1] = points[4+1]+QPointF((r*2/5), (r*2/5)*sqrt(3.0))/2;
    points[5+1] = points[4+1]+QPointF((r/5), (r/5)*sqrt(3.0))/2;

    // 外围圆
    painter.setBrush(Qt::transparent);
    painter.setPen(QPen(QBrush(Qt::black), 1.8, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawEllipse(QRectF(points[4+1]-QPointF(r,r),QSize(2*r, 2*r)));
    painter.setPen(QPen(QBrush(Qt::blue), 1.8, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(points[4+1]+QPointF(r/2.4, sqrt(r*r-r*r/2.4/2.4)), points[4+1]+QPointF(-r/2.4, sqrt(r*r-r*r/2.4/2.4)));

    //30度线
    painter.setPen(QPen(QBrush(Qt::black), 1.4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(points[4+1]-QPointF((r/2),(r/2)*sqrt(3.0)),points[4+1]+QPointF((r/2),(r/2)*sqrt(3.0)));

    //1,2,3,4,5,6,7,8,9点
    painter.setFont(QFont(QString::fromLocal8Bit("黑体"), r/150*16));
    QPointF adjustPoint(r/150*13,r/150*10);
    for(int i=1;i<=9;i++)
    {
        if (i == currentPoint)
        {
            painter.setPen(QPen(Qt::red));
            painter.setBrush(Qt::red);
        }
        else
        {
            painter.setPen(QPen(Qt::black));
            painter.setBrush(Qt::black);
        }
        painter.drawEllipse(points[i], 3, 3);
        painter.drawText(points[i]+adjustPoint, QString("%0").arg(i));
    }

    painter.setFont(QFont(QString::fromLocal8Bit("黑体"), r/120*12));
    painter.setBrush(QBrush(QColor(0, 255, 255, 150)));

    for(It it=data.begin();it!=data.end();it++)
    {
        char str[100];
        sprintf_s(str, "%.3lf", it.value());
        painter.setPen(Qt::white);
        painter.drawRoundRect(QRect((points[it.key()]+QPointF(r/120*25,-r/120*4)).toPoint(), QSize(r/120*50,r/120*15)), 15, 15);
        painter.setPen(Qt::blue);
        painter.drawText(QRect((points[it.key()]+QPointF(r/120*25,-r/120*4)).toPoint(), QSize(r/120*50,r/120*15)), Qt::AlignCenter, str);
    }
}

void PointPlan::paintPlanF()//单点测量
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int width = this->width();
    int height = this->height();
    int shortEdge = min(width, height);

    QPointF points[2];
    // 圆心
    points[0+1]= QPointF(width/2.0, height/2.0);
    // 半径
    double r = (shortEdge - 10)/2.0;

    // 外围圆
    painter.setBrush(Qt::transparent);
    painter.setPen(QPen(QBrush(Qt::black), 1.8, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawEllipse(QRectF(points[0+1]-QPointF(r,r),QSize(2*r, 2*r)));
    painter.setPen(QPen(QBrush(Qt::blue), 1.8, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(points[0+1]+QPointF(r/2.4, sqrt(r*r-r*r/2.4/2.4)), points[0+1]+QPointF(-r/2.4, sqrt(r*r-r*r/2.4/2.4)));
    //圆心点
    painter.setFont(QFont(QString::fromLocal8Bit("黑体"), r/150*16));
    QPointF adjustPoint(-r/30,-5);
    for(int i=1;i<=1;i++)
    {
        if (i == currentPoint)
        {
            painter.setPen(QPen(Qt::red));
            painter.setBrush(Qt::red);
        }
        else
        {
            painter.setPen(QPen(Qt::black));
            painter.setBrush(Qt::black);
        }
        painter.drawEllipse(points[i], 3, 3);
        painter.drawText(points[i]+adjustPoint, QString("%0").arg(i));
    }

    painter.setFont(QFont(QString::fromLocal8Bit("黑体"), r/120*12));
    painter.setBrush(QBrush(QColor(0, 255, 255, 150)));

    for(It it=data.begin();it!=data.end();it++)
    {
        char str[100];
        sprintf_s(str, "%.3lf", it.value());
        painter.setPen(Qt::white);
        painter.drawRoundRect(QRect((points[it.key()]+QPointF(-r/5,4)).toPoint(), QSize(r/120*50,r/120*15)), 15, 15);
        painter.setPen(Qt::blue);
        painter.drawText(QRect((points[it.key()]+QPointF(-r/5,4)).toPoint(), QSize(r/120*50,r/120*15)), Qt::AlignCenter, str);
    }
}

void PointPlan::paintPlanG()//十八点测量方法
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int width = this->width();
    int height = this->height();
    int shortEdge = min(width, height);

    QPointF points[26];
    // 圆心
    points[24+1]= QPointF(width/2.0, height/2.0);
    // 半径
    double r = (shortEdge - 10)/2.0;

    points[0+1] = points[24+1]-QPointF(0, (r/5));
    points[1+1] = points[24+1]+QPointF((r/5),-(r/5)*sqrt(3.0))/2;
    points[2+1] = points[24+1]+QPointF((r/5)*sqrt(3.0),-(r/5))/2;
    points[3+1] = points[24+1]+QPointF((r/5),0 );
    points[4+1] = points[24+1]+QPointF((r/5)*sqrt(3.0),(r/5))/2;
    points[5+1] = points[24+1]+QPointF((r/5),(r/5)*sqrt(3.0))/2;
    points[18+1] = points[24+1]+QPointF(0, (r/5));
    points[19+1] = points[24+1]+QPointF(-(r/5),(r/5)*sqrt(3.0))/2;
    points[20+1] = points[24+1]+QPointF(-(r/5)*sqrt(3.0),(r/5))/2;
    points[21+1] = points[24+1]+QPointF(-(r/5),0 );
    points[22+1] = points[24+1]+QPointF(-(r/5)*sqrt(3.0),-(r/5))/2;
    points[23+1] = points[24+1]+QPointF(-(r/5),-(r/5)*sqrt(3.0))/2;
    points[6+1] = points[24+1]-QPointF(0, (r/2));
    points[7+1] = points[24+1]+QPointF((r/2)*sqrt(3.0),-(r/2))/2;
    points[8+1] = points[24+1]+QPointF((r/2)*sqrt(3.0),(r/2))/2;
    points[9+1] = points[24+1]+QPointF(0, (r/2));
    points[10+1] = points[24+1]+QPointF(-(r/2)*sqrt(3.0),(r/2))/2;
    points[11+1] = points[24+1]+QPointF(-(r/2)*sqrt(3.0),-(r/2))/2;
    points[12+1] = points[24+1]-QPointF(0, (r*4/5));
    points[13+1] = points[24+1]+QPointF((r*4/5)*sqrt(3.0),-(r*4/5))/2;
    points[14+1] = points[24+1]+QPointF((r*4/5)*sqrt(3.0),(r*4/5))/2;
    points[15+1] = points[24+1]+QPointF(0, (r*4/5));
    points[16+1] = points[24+1]+QPointF(-(r*4/5)*sqrt(3.0),(r*4/5))/2;
    points[17+1] = points[24+1]+QPointF(-(r*4/5)*sqrt(3.0),-(r*4/5))/2;

    // 外围圆
    painter.setBrush(Qt::transparent);
    painter.setPen(QPen(QBrush(Qt::black), 1.8, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawEllipse(QRectF(points[24+1]-QPointF(r,r),QSize(2*r, 2*r)));
    painter.setPen(QPen(QBrush(Qt::blue), 1.8, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(points[24+1]+QPointF(r/2.4, sqrt(r*r-r*r/2.4/2.4)), points[24+1]+QPointF(-r/2.4, sqrt(r*r-r*r/2.4/2.4)));

    // 内部圆
    painter.setPen(QPen(QBrush(Qt::black), 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawEllipse(QRectF(points[24+1]-QPointF(r/2,r/2),QSize(r, r)));
    painter.drawLine(points[24+1]+QPointF(0, r), points[24+1]-QPointF(0, r));

    //边缘6mm圆
    painter.setPen(QPen(QBrush(Qt::black), 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawEllipse(QRectF(points[24+1]-QPointF(r*4/5,r*4/5),QSize(8*r/5,8*r/5)));
    painter.drawLine(points[24+1]+QPointF(0, r*4/5), points[24+1]-QPointF(0, r*4/5));

    // 内部线
    painter.setPen(QPen(QBrush(Qt::black), 1.4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(points[0+1], points[18+1]);
    painter.drawLine(points[1+1], points[19+1]);
    painter.drawLine(points[2+1], points[20+1]);
    painter.drawLine(points[3+1], points[21+1]);
    painter.drawLine(points[4+1], points[22+1]);
    painter.drawLine(points[5+1], points[23+1]);

    //1，2，3，4，5,6,7,8,9,10,11,12,13,14,15,16,17,18点
    painter.setFont(QFont(QString::fromLocal8Bit("黑体"), r/150*16));
    QPointF adjustPoint(-r/10,-r/9-7);
    QPointF adjustPoint2(r/10,-7);
    for(int i=1;i<=25;i++)
    {
        if (i == currentPoint)
        {
            painter.setPen(QPen(Qt::red));
            painter.setBrush(Qt::red);
        }
        else
        {
            painter.setPen(QPen(Qt::black));
            painter.setBrush(Qt::black);
        }
        if(i<19)
        {   painter.drawEllipse(points[i], 3, 3);
            painter.drawText(QRectF(points[i]+adjustPoint, points[i]+adjustPoint2),
                             Qt::AlignCenter, QString("%0").arg(i));
        }
        else
        {
            painter.drawEllipse(points[i], 3, 3);
        }

    }

    painter.setFont(QFont(QString::fromLocal8Bit("黑体"), r/120*12));
    painter.setBrush(QBrush(QColor(0, 255, 255, 150)));

    for(It it=data.begin();it!=data.end();it++)
    {
        char str[100];
        sprintf_s(str, "%.3lf", it.value());

        if(it.key()<=6)
        {
            switch(it.key())
            {
            case 1:
                painter.setPen(Qt::white);
                painter.drawRoundRect(QRect((points[it.key()]+QPointF(-r*10/50,-r/6)).toPoint(), QSize(r/120*50,r/120*15)), 15, 15);
                painter.setPen(Qt::blue);
                painter.drawText(QRect((points[it.key()]+QPointF(-r*10/50,-r/6)).toPoint(), QSize(r/120*50,r/120*15)), Qt::AlignCenter, str);
                break;
            case 2:
                painter.setPen(Qt::white);
                painter.drawRoundRect(QRect((points[it.key()]+QPointF(-r*90/300,-r/15)).toPoint(), QSize(r/120*50,r/120*15)), 15, 15);
                painter.setPen(Qt::blue);
                painter.drawText(QRect((points[it.key()]+QPointF(-r*90/300,-r/15)).toPoint(), QSize(r/120*50,r/120*15)), Qt::AlignCenter, str);
                break;
            case 3:
                painter.setPen(Qt::white);
                painter.drawRoundRect(QRect((points[it.key()]+QPointF(-r*113/300,-r/100)).toPoint(), QSize(r/120*50,r/120*15)), 15, 15);
                painter.setPen(Qt::blue);
                painter.drawText(QRect((points[it.key()]+QPointF(-r*113/300,-r/100)).toPoint(), QSize(r/120*50,r/120*15)), Qt::AlignCenter, str);
                break;
            case 4:
                painter.setPen(Qt::white);
                painter.drawRoundRect(QRect((points[it.key()]+QPointF(-r*121/300,r/50)).toPoint(), QSize(r/120*50,r/120*15)), 15, 15);
                painter.setPen(Qt::blue);
                painter.drawText(QRect((points[it.key()]+QPointF(-r*121/300,r/50)).toPoint(), QSize(r/120*50,r/120*15)), Qt::AlignCenter, str);
                break;
            case 5:
                painter.setPen(Qt::white);
                painter.drawRoundRect(QRect((points[it.key()]+QPointF(-r*113/300,r/20)).toPoint(), QSize(r/120*50,r/120*15)), 15, 15);
                painter.setPen(Qt::blue);
                painter.drawText(QRect((points[it.key()]+QPointF(-r*113/300,r/20)).toPoint(), QSize(r/120*50,r/120*15)), Qt::AlignCenter, str);
                break;
            case 6:
                painter.setPen(Qt::white);
                painter.drawRoundRect(QRect((points[it.key()]+QPointF(-r*91/300,r/10)).toPoint(), QSize(r/120*50,r/120*15)), 15, 15);
                painter.setPen(Qt::blue);
                painter.drawText(QRect((points[it.key()]+QPointF(-r*91/300,r/10)).toPoint(), QSize(r/120*50,r/120*15)), Qt::AlignCenter, str);
                break;
            }
        }
        else
        {
            painter.setPen(Qt::white);
            painter.drawRoundRect(QRect((points[it.key()]+QPointF(-r/5,4)).toPoint(), QSize(r/120*50,r/120*15)), 15, 15);
            painter.setPen(Qt::blue);
            painter.drawText(QRect((points[it.key()]+QPointF(-r/5,4)).toPoint(), QSize(r/120*50,r/120*15)), Qt::AlignCenter, str);
        }
    }
}

void PointPlan::paintPlanH()//中心点测量法（验机用）
{

}

PointPlan::PointPlans PointPlan::getCurrentPlan() const
{
    return currentPlan;
}

void PointPlan::setCurrentPlan(const PointPlans &value)
{
    currentPlan = value;
    clearData();
}

void PointPlan::setCurrentPlan(const QString &value)
{
    if(~value.indexOf("A")) setCurrentPlan(PlanA);
    if(~value.indexOf("B")) setCurrentPlan(PlanB);
    if(~value.indexOf("C")) setCurrentPlan(PlanC);
    if(~value.indexOf("D")) setCurrentPlan(PlanD);
    if(~value.indexOf("E")) setCurrentPlan(PlanE);
    if(~value.indexOf("F")) setCurrentPlan(PlanF);
    if(~value.indexOf("G")) setCurrentPlan(PlanG);
    if(~value.indexOf("H")) setCurrentPlan(PlanH);
}

int PointPlan::getCurrentPoint() const
{
    return currentPoint;
}

void PointPlan::setCurrentPoint(int value)
{
    currentPoint = value;
    update();
}

QList<int> PointPlan::getCenterPoint()
{
    QList<int> centerPoint;

    switch(currentPlan)
    {
    case PlanA: centerPoint << 1;break;
    case PlanB: centerPoint << 2 << 5;break;
    case PlanC: centerPoint << 3 << 8;break;
    case PlanD: centerPoint << 2 << 5;break;
    case PlanE: centerPoint << 5;break;
    case PlanF: centerPoint << 1;break;
    case PlanG: centerPoint << 1 << 2 << 3 << 4 << 5 << 6;break;
    case PlanH: break;
    }

    return centerPoint;
}

void PointPlan::addData(int x, double value)
{
    data[x] = value;
    update();
}

void PointPlan::clearData()
{
    data.clear();
    currentPoint = 0;
    update();
}

