#include "controler.h"

#include <QSplashScreen>
/*
 * Controler类，实现对界面，通信，数据的总控制。
 * 为所控制类的友元类，具备完整的访问权限
 */

Controler::Controler(QObject *parent) :
    QObject(parent)
{
    init();

    Database::init();
}

Controler::~Controler()
{
    checkFinish();
    Communication::deleteInstance();
}

void Controler::init()
{
    connect(widget.measureButton, SIGNAL(clicked()),
            this, SLOT(startGroup()));
    connect(widget.remeasureButton, SIGNAL(clicked()),
            this, SLOT(startRemeasure()));

    connect(&calculateMethod, SIGNAL(sendOneRowData(DataMap)),
            &measureStatus, SLOT(getOneRowData(DataMap)));
    connect(&measureStatus, SIGNAL(finishOnePoint(int,double)),
            &widget, SLOT(finishOnePoint(int,double)));
    connect(&measureStatus, SIGNAL(finishOneRow()),
            &widget, SLOT(doUiUnlock()));

    calculateMethod.setCurrentMethod(CalculateMethod::SLICE_METHOD);
    widget.setResultTableHeader(calculateMethod.getCurrentHeader());
}

void Controler::checkFinish()
{
    if(measureStatus.isFinishGroup())
    {
        qDebug() << "Finish Group";
        writeToDatabase();
        measureStatus.clearAllRow();
        widget.finishedGroupMeasure();
    }
}

void Controler::writeToDatabase()
{
    QStringList headers;
    QTableWidget *informationWidget = Widget::getInformationTable();
    for(int i=0;i<informationWidget->columnCount();i++)
        headers << informationWidget->horizontalHeaderItem(i)->text();

    QTableWidget *tableWidget = Widget::getResultTable();
    int colCount = tableWidget->columnCount();
    for(int i=0;i<colCount;i++)
        headers << tableWidget->horizontalHeaderItem(i)->text();

    QString query = "CREATE TABLE [Test] ( ";
    foreach(QString name, headers)
    {
        query+=QString("[%0] TEXT").arg(name);

        if(name==headers.last())
            query+=")";
        else
            query+=",";
    }
    Database::Query(query);

    int rowCount = tableWidget->rowCount();
    for(int i=0;i<rowCount;i++) if(tableWidget->item(i, 0))
    {
        query = "insert into [Test] values(";
        for(int j=0;j<informationWidget->columnCount();j++)
            query += QString("'%0',").arg(informationWidget->item(0,j)->text());

        for(int j=0;j<colCount;j++)
        {
            query += QString("'%0'").arg(tableWidget->item(i,j)->text());
            if(j==colCount-1)
                query+=")";
            else
                query+=",";
        }
        qDebug() << query;
        Database::Query(query);
    }
}

void Controler::startGroup()
{
    checkFinish();
    startMeasure();
}

void Controler::startMeasure()
{
    if(!widget.checkInput()) return;    // 检查输入框是否输入完整
    widget.doUiLock();                  // 锁定Ui控件，防止用户更改

    calculateMethod.setParameter(widget.parameterMap);
    measureStatus.setParameterMap(widget.parameterMap);

    int currentPoint = measureStatus.getCurrentPoint();
    widget.setCurrentPoint(currentPoint);
    calculateMethod.setCurrentPoint(currentPoint);

    doMeasure();             // 测量开始
}

void Controler::startRemeasure()
{
    measureStatus.clearPreRow();
    startMeasure();
}

void Controler::doMeasure(CombinationMethod combinationMethod)
{
    int pulseWidth = widget.parameterMap[str("脉宽")].toInt();    // 构造脉宽指令
    Measure *measure = new Measure(QByteArray(1, (char)(0xC0 | pulseWidth)));
    connect(measure,SIGNAL(sendV(double,double)),
            &calculateMethod, SLOT(getV(double,double)));
    connect(measure,SIGNAL(sendV(double,double)),
            &widget, SLOT(changeV23(double,double)));

    if(combinationMethod == DoubleCombination)                   // 如果是双组合的测量方式
        connect(measure, SIGNAL(workFinished()),                 // 测量下一组
                this, SLOT(doMeasureAgain()));
    measure->start();
}

void Controler::doMeasureAgain()
{
    int pulseWidth = widget.parameterMap[str("脉宽")].toInt();    // 构造脉宽指令
    Measure *measure = new Measure(QByteArray(1, (char)(0xE0 | pulseWidth)));
    connect(measure,SIGNAL(sendV(double,double)),
            &calculateMethod, SLOT(getV(double,double)));
    connect(measure,SIGNAL(sendV(double,double)),
            &widget, SLOT(changeV24(double,double)));
    measure->start();
}
