#include "tablica.h"
#include <QtGui>
#include <QLabel>
#include <QDebug>
#include <QFrame>
#include <clickablelabel.h>

Tablica::Tablica(QWidget *parent):
    QFrame(parent)
{

    // zainicjowanie tablicy 3x3
    rowNumber = 3;
    colNumber = 3;
    squareNumber = 0;

    for (int i = 0; i < 9; i++)
    {
        addLabel(i);
    }
}

// funkcja dodająca nowy label do listy
void Tablica::addLabel(int n)
{
    ClickableLabel* mylabel = new ClickableLabel(this);
    labelList.append(mylabel);
    //labelList.at(n)->setText(QString::number(n));
    labelList.at(n)->setAlignment(Qt::AlignCenter);
    labelList.at(n)->setStyleSheet("QLabel { background-color : yellow; color : blue; }");
    squareNumber++;

}

// funkcja usuwająca label z listy
void Tablica::deleteLabel(int n)
{
    delete labelList.at(n);
    labelList.removeAt(n);
}
