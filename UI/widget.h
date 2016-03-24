#ifndef WIDGET_H
#define WIDGET_H

// qt ui core
#include <QWidget>
#include <QMessageBox>
#include <QTableWidget>
#include <QDebug>

// own
#include "inputedit.h"
#include "pointplan.h"
#include "global.h"

namespace Ui {class Widget;}

class SHARED_EXPORT Widget : public QWidget
{
    Q_OBJECT
    friend class Controler;                             // Controler友元，便于访问

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    void createInputEditTable();                        // 建立输入框控件表
    void readInputEditSettings();                       // 从注册表中读取输入框内容
    void saveInputEditSettings();                       // 存储输入框内容到注册表
    void createShowValueTable();                        // 建立显示电压控件表
    bool checkInput();                                  // 检查输入框中是否有数据

    void setCurrentPoint(int point);                    // 设定当前测量点，以高亮

    void setResultTableHeader(QStringList headers);

    // static
    static QTableWidget* getResultTable();              // 获取结果表
    static QTableWidget* getInformationTable();         // 获取信息表
    static Widget* instance();                          // 返回当前单例
    static PointPlan *getPointPlan();                   // 获取选点方案图

public slots:
    void finishOnePoint(int, double);                   // 完成一个点的数据，更新值
    void finishedGroupMeasure();                        // 完成一组测量，更新值
    void updateAnalysisTableWidget();                   // 更新统计表

    void updateInputParameter(const QString &);         // 根据输入框内容更新表中数据

    void changeV23(double v23, double v32);             // 动态改变电压值
    void changeV24(double v24, double v42);

    void doUiLock();                                    // 锁定UI，防止用户修改
    void doUiUnlock();                                  // 解锁UI

    void on_aboutButton_clicked();
    void on_resistanceRadioButton_clicked();
    void on_resistivityRadioButton_clicked();

    void on_measureButton_clicked();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::Widget *ui;                                     // ui控件
    PointPlan *pointPlan;                               // 选点方案图

    QMap<QString, InputEdit*> inputEditTable;           // 记录输入框控件
    QMap<QString, QLineEdit*> showValueTalbe;           // 显示电压值控件

    QStringMap parameterMap;                            // 空间参数保存

    static Widget* self;                                // 当前单例

    QPushButton *measureButton;
    QPushButton *remeasureButton;
};

#endif // WIDGET_H
