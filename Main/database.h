#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QDebug>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlResult>
#include <QSqlRecord>
#include <QSqlField>

namespace DB
{
    typedef QList<QVariant> Data;
    typedef QList<Data> DataList;
}

using namespace DB;
class Database : public QObject
{
    Q_OBJECT
private:
    static QSqlQuery* query;
    static DataList result;
    static QStringList headers;

private:
    static void clearData();

public:
    static bool init();
    static bool Query(QString order);

    static DataList& getResult();
    static QStringList& getHeaders();

public:

private:


};

#endif // DATABASE_H
