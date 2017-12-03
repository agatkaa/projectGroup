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

ClickableLabel::ClickableLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent) {

     connect(this, SIGNAL(clicked()),this, SLOT(slotClicked()));
    tab = (Tablica*)parent;

     setAcceptDrops(true);
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

void ClickableLabel::slotClicked()
{
//   Repository* repository = new Repository();
//   Image* img = repository->getImages()[0];

//   QPixmap pixmap(img->fullFileName);
//   this->setPixmap(pixmap);
//   this->setScaledContents( true );
//   this->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
//   update();
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

void ClickableLabel::dragEnterEvent(QDragEnterEvent *event)
{
   // event->accept();
    event->setAccepted(true);

    this->setStyleSheet("border: 1px solid red; background-color: yellow");
}

void ClickableLabel::dropEvent(QDropEvent *event)
{
    QString imageLocation = event->mimeData()->text();
    QPixmap pixmap(imageLocation);
    this->setPixmap(pixmap);
    this->setScaledContents( true );
    this->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    this->setStyleSheet("border: none; background-color: yellow");
    update();
    QString path = imageLocation.section('/', -1);
    Repository* rep = new Repository();

    rep -> saveImageInTable(tab->table.id,path,this->xPosition,this->yPosition);

}
void ClickableLabel::dragLeaveEvent(QDragLeaveEvent *event)
{
    this->setStyleSheet("border: none; background-color: yellow");
}


