#ifndef TABLICA_H
#define TABLICA_H

#include <QObject>
#include <QWidget>
#include <QFrame>
#include <QLabel>

class Tablica : public QFrame
{
    Q_OBJECT

public:
    explicit Tablica(QWidget *parent);
    //~Tablica();

public:
    void addLabel(int n);
    void deleteLabel(int n);
    QList <QLabel *> labelList;
    int rowNumber;
    int colNumber;
    int squareNumber;
};

#endif // TABLICA_H
