#include "tablica.h"
#include "clickablelabel.h"
#include <QDebug>
#include <mainwindow.h>
#include <QPixmap>
#include <QFileDialog>
#include <QDir>
#include <repository.h>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDebug>
#include <QSettings>
#include <QDrag>

ClickableLabel::ClickableLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent) {

     connect(this, SIGNAL(clicked()),this, SLOT(slotClicked()));
    tab = (Tablica*)parent;

     setAcceptDrops(true); // ustaawienie akceptacji upuszczenia elementu w drag and drop
}

void ClickableLabel::setYPosition(int value)
{
    yPosition = value;
}

void ClickableLabel::setXPosition(int value)
{
    xPosition = value;
}

ClickableLabel::~ClickableLabel() {}

void ClickableLabel::mousePressEvent(QMouseEvent* event) {
    emit clicked();
}

//  funkcja dla przesuwania obrazków pomiędzy labelami
void ClickableLabel::slotClicked()
{
    if(this->imageLocation!=""){    // sprawdamy czy w danym labelu jest obrazek
        QDrag *drag = new QDrag(this);  // utworzenie obiektu typu drag
        imageLocation = this->imageLocation;    // przypisanie lokalizacji obrazka
        QMimeData *mime = new QMimeData;    // dane przypisane do operacji drag and drop
        mime->setText(imageLocation);   //  dodanie lokazlizacji obrazka do danych
        drag->setMimeData(mime);    // ustawienie powyższych danych do obiektu drag
        drag->exec();   //  rozpoczęcie akcji drag and drop
    }
}

QString ClickableLabel::openFile()
 {
    QString filename =  QFileDialog::getOpenFileName(
              this,
              "Open Document",
              QDir::currentPath(),
              "Image Files (*.png *.jpg *.bmp *.jpeg)");
   if( !filename.isNull() )
   {
     return filename;
   }
 }

// akcja w momencie najechania nad obiekt przy akcji drag and drop
void ClickableLabel::dragEnterEvent(QDragEnterEvent *event)
{
   // event->accept();
    event->setAccepted(true); // opcja pozwalająca na na akcję drag and drop na obiekcie
    this->setStyleSheet("border: 1px solid red; background-color: yellow");     // zmiana obwódki na czerwoną, gdy chcemy coś upuścić nad labelem
}

// akcja w momencie upuszczenia obiektu typu drag and drop nad danym labelem
void ClickableLabel::dropEvent(QDropEvent *event)
{
    imageLocation = event->mimeData()->text();  // przypisanie obrazka znajdującego się w mimeData (z drag and drop) do labelka nad którym upuszczamy obiekt
    QPixmap pixmap(imageLocation);  // dodanie przeniesionego obrazka do labela
    this->setPixmap(pixmap);
    this->setScaledContents( true );
    this->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    this->setStyleSheet("border: none; background-color: yellow");
    update();
    QString path = imageLocation.section('/', -1);

    tab->repository -> saveImageInTable(tab->table.id,path,this->xPosition,this->yPosition);

}

// akcja w momencie zjechania z labelka dla akcji dag and drop
void ClickableLabel::dragLeaveEvent(QDragLeaveEvent *event)
{
    this->setStyleSheet("border: none; background-color: yellow");  // usunięcie czerwonej obwódki
}


