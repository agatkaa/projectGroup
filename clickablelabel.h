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
    void slotClicked(); // funkcja umożliwiająca przerzucanie obrazka z jednego labela do drugiego

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event); // akcja w momencie kliknięcia okienka na obrazki
    void dragEnterEvent(QDragEnterEvent *event); // akcja w momencie najechania nad obiekt przy akcji drag and drop
    void dropEvent(QDropEvent *event); // akcja w momencie puszczenia przycisku myszy nad obiektem w akcji drag and drop
    void dragLeaveEvent(QDragLeaveEvent *event); // akcja w momencie zjechania myszką znad obiektu w akcji drag and drop
    QString imageLocation;

public:
     QString openFile();

     void setXPosition(int value);

     void setYPosition(int value);

private:

};

#endif // CLICKABLELABEL_H
