#ifndef TABLE_H
#define TABLE_H

#include <QString>
#include <QDate>

class Table
{
public:
    Table();
    int id;
    QString name;
    int rowNumber;
    int colNumber;
    QDate lastUpdate;

    QString displayName();
};

#endif // TABLE_H
