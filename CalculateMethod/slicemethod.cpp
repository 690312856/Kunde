#include "slicemethod.h"

double SliceMethod::formulaCalculate(QStringMap& parameterMap, QList<double>& v)
{
    double I = parameterMap[str("电流")].toDouble();
    double W = parameterMap[str("厚度")].toDouble();

    double Ra = (v[0]+v[1])/I/2.0;
    double Rb = (v[2]+v[3])/I/2.0;

    double p0 = (-14.696+25.173*Ra/Rb-7.872*(Ra/Rb)*(Ra/Rb))*Ra*W;

    return p0;
}

double SliceMethod::resultsCorrected(QStringMap &parameterMap, QList<double> &v)
{
    double p0 = formulaCalculate(parameterMap, v);

    double W = parameterMap[str("厚度")].toDouble();
    double S = parameterMap[str("间距")].toDouble();

    // 厚度修正
    if (W < 4*S)
        p0 *= ReadData::thicknessCorrect(W/S);                  // 四倍内修正

    return p0;
}

double SliceMethod::temperatureCorrected(QStringMap &parameterMap, double p0)
{
    // 温度修正
    double T = parameterMap[str("温度")].toDouble();
    if(parameterMap[str("类型")].indexOf("N") != -1)
        p0 *= 1-ReadData::temperatureNCorrect(p0)*(T-23);       // N型
    else
        p0 *= 1-ReadData::temperaturePCorrect(p0)*(T-23);       // P型

    return p0;
}

void SliceMethod::calculate(QStringMap& parameterMap, QList<double>& v, DataMap& data)
{
    double result = resultsCorrected(parameterMap, v);
    data[str("电阻率")] = result;
    result = temperatureCorrected(parameterMap, result);
    data[str("result")] = data[str("温度修正后")] = result;
}
