#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QWidget>

class Tablica;
#include "table.h"
#include "repository.h"
#include <Qt>

// klasa , która definiuje 'labele' pojedyncze kwadraty, umożliwiająca klikanie w labele
class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    int xPosition;
    int yPosition;
    ~ClickableLabel();
    Tablica* tab;

public slots:
    void slotClicked();

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    QString imageLocation;

public:
     QString openFile();

     void setXPosition(int value);

     void setYPosition(int value);

private:

};

#endif // CLICKABLELABEL_H
