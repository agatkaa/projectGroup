#ifndef TABLICA_H
#define TABLICA_H

#include <QObject>
#include <QWidget>
#include <QFrame>

#include "clickablelabel.h"
#include "table.h"
#include "repository.h"


class Tablica : public QFrame
{
    Q_OBJECT

public:
    explicit Tablica(QWidget *parent, Repository* repository);
    ~Tablica();

public:
    void addLabel(int n, int xPosition, int yPosition);
    void deleteLabel(int n);
    void saveTable();
    QList <ClickableLabel *> labelList;
    Table table;
    int squareNumber;
    void initializeTable();
private:
    Repository* repository;
};

#endif // TABLICA_H
