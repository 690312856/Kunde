#include "database.h"

QSqlQuery* Database::query = NULL;
DataList Database::result;
QStringList Database::headers;

void Database::clearData()
{
    int s=result.size();
    while(s--)
        result[s].clear();
    result.clear();
    headers.clear();
}

bool Database::init()
{
    static QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("main.db");
    if(!db.open())
    {
        qDebug() << "Open file false";
        return false;
    }

    query = new QSqlQuery(db);
    if(query) return true;
    return false;
}

bool Database::Query(QString order)
{
    clearData();

    query->clear();
    query->exec(order);

    if (!query->isActive()) return false;

    QMap<QString, QString> nameMapper;
    nameMapper["Name"]=QString::fromLocal8Bit("姓名");
    nameMapper["Index"]=QString::fromLocal8Bit("序号");
    nameMapper["Date"]=QString::fromLocal8Bit("日期");
    nameMapper["Value"]=QString::fromLocal8Bit("数值");
    nameMapper["Result"]=QString::fromLocal8Bit("结果");

    int count = query->record().count();
    for(int i=0;i<count;i++)
        headers << nameMapper[query->record().field(i).name()];

    while(query->next())
    {
        Data data;
        for(int i=0;i<count;i++)
            data.append(query->value(i));
        result.append(data);
    }

    return true;
}

DataList &Database::getResult()
{
    return result;
}

QStringList &Database::getHeaders()
{
    return headers;
}
