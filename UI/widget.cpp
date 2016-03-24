
#include "widget.h"
#include "ui_widget.h"

Widget* Widget::self = 0;

Widget::Widget(QWidget *parent) :
    QWidget(parent)
  , ui(new Ui::Widget)
{
    self = this;

    ui->setupUi(this);
    measureButton = ui->measureButton;
    remeasureButton = ui->remeasureButton;

    pointPlan = new PointPlan(this);
    ui->pointPlan->addWidget(pointPlan);
    connect(ui->pointPlanEdit, SIGNAL(currentTextChanged(QString)),
            pointPlan, SLOT(setCurrentPlan(QString)));

    this->createInputEditTable();
    this->createShowValueTable();

    ui->picOnelabel->setPixmap(QPixmap("://1.jpg").scaled(160,76));
    ui->picTwolabel->setPixmap(QPixmap("://2.png").scaled(160,76));

    ui->informationTableWidget->verticalHeader()->setStretchLastSection(true);
    ui->informationTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->informationTableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->informationTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->analysisTableWidget->verticalHeader()->setStretchLastSection(true);
    ui->analysisTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->analysisTableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->analysisTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->resultTableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->resultTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->resultTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->resultTableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    ui->resistivityRadioButton->click();
    readInputEditSettings();
}

Widget::~Widget()
{
    saveInputEditSettings();
    delete ui;
}

void Widget::createInputEditTable()
{
    inputEditTable.clear();

    QObjectList inputObjectList = ui->inputGroup->children()        // 输入参数框
                                + ui->pointPlanGroup->children();   // 选点方案框

    foreach(QObject *obj, inputObjectList)
    {
        if(obj->inherits("QLineEdit") || obj->inherits("QComboBox"))
        {
            InputEdit * edit = new InputEdit(obj, this);
            connect(edit, SIGNAL(textChanged(QString)),
                    this, SLOT(updateInputParameter(QString)));

            inputEditTable[edit->name()] = edit;
        }
    }
}

void Widget::createShowValueTable()
{
    showValueTalbe.clear();
    QObjectList showValueObjectList = ui->showValueGroup->children();
    foreach(QObject *obj, showValueObjectList)
    {
        if(obj->inherits("QLineEdit"))
        {
            QLineEdit * edit = qobject_cast<QLineEdit*>(obj);
            showValueTalbe[edit->toolTip()] = edit;
        }
    }
}

bool Widget::checkInput()
{
    foreach(InputEdit * edit, inputEditTable)
    {
        if(edit->text().isEmpty())
        {
            QMessageBox::information(NULL, str("提示"), str("请填写完整!\n") +
                                     edit->name()+str("框未填写"));
            return false;
        }
    }
    return true;
}

void Widget::setCurrentPoint(int point)
{
    pointPlan->setCurrentPoint(point);
}

void Widget::setResultTableHeader(QStringList headers)
{
    // 清理列表已有内容
    ui->resultTableWidget->clearContents();
    // 自动调整大小
    ui->resultTableWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    // 设定列数
    ui->resultTableWidget->setColumnCount(headers.length());
    // 设定行数
    ui->resultTableWidget->setRowCount(10);
    // 设定表头
    ui->resultTableWidget->setHorizontalHeaderLabels(headers);
}

void Widget::doUiLock()
{
    ui->measureButton->setDisabled(true);
    ui->remeasureButton->setDisabled(true);
    ui->measureButton->setText(str("正在测量.."));

    // 清空显示的电压值
    foreach(QLineEdit * edit, showValueTalbe)
        edit->clear();
}

void Widget::doUiUnlock()
{
    ui->measureButton->setDisabled(false);
    ui->remeasureButton->setDisabled(false);
    ui->measureButton->setText(str("测量完成！"));
    ui->resultTableWidget->setFocus();
}

void Widget::changeV23(double v23, double v32)
{
    v23 = abs(v23);
    v32 = abs(v32);

    ui->v23PositiveEdit->setText(QString("%0").arg(v23));
    ui->v23NegativeEdit->setText(QString("%0").arg(v32));
    ui->v23AverageEdit->setText(QString("%0").arg((v23+v32)/2.0));
}

void Widget::changeV24(double v24, double v42)
{
    v24 = abs(v24);
    v42 = abs(v42);

    ui->v24PositiveEdit->setText(QString("%0").arg(v24));
    ui->v24NegativeEdit->setText(QString("%0").arg(v42));
    ui->v24AverageEdit->setText(QString("%0").arg((v24+v42)/2.0));
}

void Widget::readInputEditSettings()
{
    QSettings settings(str("SF-_-"), str("KundeKDY"));
    foreach(InputEdit * edit, inputEditTable)
    {
        QString value = settings.value(edit->name()).toString();
        edit->setText(value);
    }
}

void Widget::saveInputEditSettings()
{
    QSettings settings(str("SF-_-"), str("KundeKDY"));
    foreach(InputEdit * edit, inputEditTable)
    {
        settings.setValue(edit->name(), edit->text());
    }
}

void Widget::updateInputParameter(const QString & value)
{
    InputEdit * now = dynamic_cast<InputEdit *>(sender());
    QString name = now->name();

    // 更新参数表
    parameterMap[name] = value;

    // 更新information表格
    QTableWidget * information = ui->informationTableWidget;
    int columnCount = information->columnCount();
    for(int i=0;i<columnCount;i++)
    {
        QString str = (QString)information->horizontalHeaderItem(i)->text();
        if (str.indexOf(name)!=-1)
            information->setItem(0, i, new QTableWidgetItem(value));
    }
}

QTableWidget* Widget::getResultTable()
{
    return self->ui->resultTableWidget;
}

QTableWidget *Widget::getInformationTable()
{
    return self->ui->informationTableWidget;
}

Widget *Widget::instance()
{
    return self;
}

PointPlan* Widget::getPointPlan()
{
    return self->pointPlan;
}

void Widget::finishOnePoint(int point, double value)
{
    pointPlan->addData(point, value);
}

// 完成一组测试
void Widget::finishedGroupMeasure()
{
    QString autoIncrement = inputEditTable[str("编号自增")]->text();
    if (autoIncrement == str("是"))
    {
        QString indexNow = inputEditTable[str("编号")]->text();
        inputEditTable[str("编号")]->setText(str("%0").arg(indexNow.toInt()+1));
    }

    pointPlan->clearData();
    ui->resultTableWidget->clearContents();
    ui->resultTableWidget->setRowCount(1);
}

void Widget::updateAnalysisTableWidget()
{
    QList<int> centerPoint = pointPlan->getCenterPoint();
    int centerPointSize = centerPoint.size();

    QList<double> result;

    int centerNum = 0;
    double pc=0;

    int rowCount = ui->resultTableWidget->rowCount();
    for(int i=0;i<rowCount;i++)
    {
        QTableWidgetItem* item = ui->resultTableWidget->item(i, 6);
        if(item==NULL) continue;
        QString res = ui->resultTableWidget->item(i, 6)->text();
        if( !res.isEmpty()) result.append(res.toDouble());

        item = ui->resultTableWidget->item(i, 0);
        int pointNow = item->text().toInt();

        foreach(int p, centerPoint)
        {
            if(p==pointNow)
            {
                pc+=res.toDouble();
                centerNum++;
                break;
            }
        }
    }

    int num = result.size();
    double minEle = *min_element(result.begin(), result.end());
    double maxEle = *max_element(result.begin(), result.end());

    double sum = 0;
    foreach(double res, result) sum+=res;
    double average = sum/num;

    double pa;
    double tmp=0;
    if(num>centerNum)
    {
       pa = (sum-pc)/(num-centerNum);
       pc /= centerNum;
       tmp = abs(pa-pc)/pc*100;
    }

    double maxPercentChange = (maxEle-minEle)/minEle*100;
    double radialRInhomogeneity = (maxEle-minEle)/(maxEle+minEle)*200;

    ui->analysisTableWidget->setItem(0, 0, new QTableWidgetItem(QString("%0").arg(maxEle)));
    ui->analysisTableWidget->setItem(0, 1, new QTableWidgetItem(QString("%0").arg(minEle)));
    ui->analysisTableWidget->setItem(0, 2, new QTableWidgetItem(QString("%0").arg(average)));
    ui->analysisTableWidget->setItem(0, 3, new QTableWidgetItem(QString("%0%").arg(maxPercentChange)));
    ui->analysisTableWidget->setItem(0, 4, new QTableWidgetItem(QString("%0%").arg(radialRInhomogeneity)));
    ui->analysisTableWidget->setItem(0, 5, new QTableWidgetItem(QString("%0%").arg(tmp)));
}

void Widget::on_resistanceRadioButton_clicked()
{
    // 薄层方块电阻
    QStringList headers;
    headers << str("晶片编号")
            << str("晶片类型")
            << str("探针间距")
            << str("探针修正系数")
            << str("温度")
            << str("厚度")
            << str("电流")
            << str("测量员");
    ui->informationTableWidget->setColumnCount(headers.length());
    ui->informationTableWidget->setHorizontalHeaderLabels(headers);
    ui->resualtTitle->setText(str("薄层方块电阻"));
}

void Widget::on_resistivityRadioButton_clicked()
{
    // 薄片电阻率
    QStringList headers;
    headers << str("晶片编号")
            << str("晶片类型")
            << str("探针间距")
            << str("探针修正系数")
            << str("温度")
            << str("厚度")
            << str("电流")
            << str("测量员");
    ui->informationTableWidget->setColumnCount(headers.length());
    ui->informationTableWidget->setHorizontalHeaderLabels(headers);
    ui->resualtTitle->setText(str("薄片电阻率"));
}

// 关于
void Widget::on_aboutButton_clicked()
{
    QMessageBox::information(this, str("关于"), str("<h1>四探针测量软件荣誉出品</h1>"));
}

void Widget::on_measureButton_clicked()
{

}

// 选点方案放大图
void Widget::on_pushButton_2_clicked()
{
    PointPlan *big = new PointPlan(this->pointPlan, NULL);
    big->setWindowTitle(QString::fromLocal8Bit("选点方案放大图"));
    big->resize(600, 600);
    connect(big, &PointPlan::finished, big, &PointPlan::deleteLater);
    big->exec();
}
