#include "measurestatus.h"
#include "controler.h"

MeasureStatus::MeasureStatus(QObject *parent) :
    QObject(parent)
{
    currentRow = 0;
    clearData();
}

void MeasureStatus::clearData()
{
    points.clear();
    pointNum = 0;
    currentPoint = 0;
}

void MeasureStatus::setPointIndex(QString pointIndex)
{
    this->clearData();

    QStringList pointList = pointIndex.split(" ", QString::SkipEmptyParts);
    foreach(QString point, pointList)
        points.append(point.toInt());

    pointNum = points.size();
    if(pointNum) currentPoint = 0;
}

void MeasureStatus::getOneRowData(DataMap data)
{
    if (currentPoint == pointNum || pointNum == 0)
    {
        setPointIndex(parameterMap[str("测量点顺序")]);
    }

    emit finishOneRow();

    // 发送一行测量完成信号
    emit finishOnePoint(points[currentPoint], data["result"]);

    QStringMap table;
    for(DataMap::iterator it=data.begin();it!=data.end();it++)
        table[it.key()] = str("%0").arg(it.value());

    table[str("测量点")] = str("%0").arg(points[currentPoint]);
    table[str("日期")] = QDate::currentDate().toString(str("yyyy-MM-dd"));
    table[str("时间")] = QTime::currentTime().toString(str("hh:mm:ss"));

    // 填充表格
    // 本来应该是UI部分，没有很好的分离开
    QTableWidget *resultTable = Widget::getResultTable();
    int rowCount = resultTable->rowCount();
    if (currentRow >= rowCount) resultTable->setRowCount(currentRow+1);
    int colCount = resultTable->columnCount();
    for(int i=0; i<colCount; i++)
    {
        resultTable->setItem(currentRow, i,
                new QTableWidgetItem(table[resultTable->horizontalHeaderItem(i)->text()]));
        resultTable->item(currentRow, i)->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    }
    resultTable->selectRow(currentRow);
    // 填充表格结束

    if(data["invalid"]==1)
        resultTable->setStyleSheet("QTableWidget::item:selected { background-color: rgb(255, 50, 50, 100); color: rgb(0,0,0)}");
    else
        resultTable->setStyleSheet("QTableWidget::item:selected { background-color: rgb(0, 0, 255, 100); color: rgb(0,0,0)}");

    Widget::instance()->updateAnalysisTableWidget();

    currentRow++;
    currentPoint++;
}

void MeasureStatus::setParameterMap(const QStringMap &value)
{
    parameterMap = value;
}

int MeasureStatus::getCurrentPoint()
{
    if (currentPoint == pointNum || pointNum == 0)
        setPointIndex(parameterMap[str("测量点顺序")]);
    return currentPoint < pointNum ? points[currentPoint] : 0;
}

bool MeasureStatus::isFinishGroup()
{
    qDebug() << currentPoint << pointNum;
    if(currentPoint == pointNum && pointNum > 0)
        return true;
    return false;
}

void MeasureStatus::clearPreRow()
{
    if(currentRow>0 && currentPoint>0)
    {
        currentRow--;
        currentPoint--;
    }
}

void MeasureStatus::clearAllRow()
{
    currentRow = 0;
    clearData();
}
