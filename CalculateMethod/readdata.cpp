#include "readdata.h"

ReadData::DataMap ReadData::thicknessData;
ReadData::DataMap ReadData::positionData;
ReadData::DataMap ReadData::temperatureNData;
ReadData::DataMap ReadData::temperaturePData;

void ReadData::readData()
{
    readCorrectedData(thicknessData, str("://厚度修正.txt"));
    readCorrectedData(positionData, str("://位置修正.txt"));
    readCorrectedData(temperatureNData, str("://温度修正表-n.txt"));
    readCorrectedData(temperaturePData, str("://温度修正表-p.txt"));
}

void ReadData::readCorrectedData(ReadData::DataMap & data, QString fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);

    QTextStream in(&file);

    while(!in.atEnd())
    {
        QStringList allData = in.readLine().split(" ", QString::SkipEmptyParts);
        if(allData.size()>=2)
            data[allData[0].toDouble()] = allData[1].toDouble();
    }

    file.close();
}

double ReadData::getCorrectedData(ReadData::DataMap & data, double key)
{
    // 直接找到数据，返回结果
    if(data.find(key) != data.end())
        return data[key];

    // 比表中最小的数据还小，返回最小数据
    if (key < data.begin().key())
        return data.begin().value();

    // 比表中最大的数据还大，返回最大数据
    if (key > (data.end()-1).key())
        return (data.end()-1).value();

    // 在表中，进行拟合
    // if you don't understand the mean of follow code, don't try to edit it!
    data[key] = 0;
    DataMap::iterator it = data.find(key);

    it--;
    double leftKey = it.key();
    double leftVal = it.value();

    it++; it++;
    double rightKey = it.key();
    double rightVal = it.value();

    it--;
    data.erase(it);

    return leftVal + (rightVal - leftVal) * (key - leftKey) / (rightKey - leftKey);
}

double ReadData::thicknessCorrect(double thickness)
{
    return getCorrectedData(thicknessData, thickness);
}

double ReadData::positionCorrect(double position)
{
    return getCorrectedData(positionData, position);
}

double ReadData::temperatureNCorrect(double temperature)
{
    return getCorrectedData(temperatureNData, temperature);
}

double ReadData::temperaturePCorrect(double temperature)
{
    return getCorrectedData(temperaturePData, temperature);
}
