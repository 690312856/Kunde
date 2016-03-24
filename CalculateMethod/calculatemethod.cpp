#include "calculatemethod.h"

#include <QSound>
CalculateMethod::CalculateMethod(QObject *parent) :
    QObject(parent)
{
}

void CalculateMethod::setParameter(QStringMap & parameterMap)
{
    this->parameterMap = parameterMap;
}

void CalculateMethod::setCurrentPoint(int currentPoint)
{
    parameterMap[str("当前点")] = currentPoint;
}

void CalculateMethod::calculate()
{
    DataMap data;

    data[str("V23+")] = v[0];
    data[str("V23-")] = v[1];
    data[str("V24+")] = v[2];
    data[str("V24-")] = v[3];

    switch (currentMethod) {
    case SLICE_METHOD:
        SliceMethod::calculate(parameterMap, v, data);
        break;
    default:
        break;
    }

    double result = data["result"];
    if (result < parameterMap[str("最小值")].toDouble() ||
        result > parameterMap[str("最大值")].toDouble())
    {
        soundTip();
        data[str("invalid")] = 1;
    }

    v.clear();
    emit sendOneRowData(data);
}

QStringList CalculateMethod::getCurrentHeader()
{
    QStringList headers;
    switch(currentMethod)
    {
    case SLICE_METHOD:
        headers << str("测量点") << str("V23+") << str("V23-") << str("V24+") << str("V24-")
                << str("电阻率") << str("温度修正后") << str("日期") << str("时间");
        break;
    default:
        break;
    }

    return headers;
}

void CalculateMethod::soundTip()
{
    // 发出提示音
    static QSound sound("://tip.wav");
    sound.play();
}

void CalculateMethod::getV(double v1, double v2)
{
    v.append(v1);
    v.append(v2);

    if(v.size()>=4) calculate();
}

void CalculateMethod::setCurrentMethod(const Method &value)
{
    currentMethod = value;
}
