#include "table.h"

Table::Table()
{
    id = -1;
}

QString Table::displayName()
{
    return QString("%1 %2[%3x%4]").arg(QString::number(id), name, QString::number(rowNumber), QString::number(colNumber));
}
