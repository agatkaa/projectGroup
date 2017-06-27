#include "clickablelabel.h"
#include <QDebug>
#include <mainwindow.h>
#include <QPixmap>
#include <QFileDialog>
#include <QDir>
#include <repository.h>

ClickableLabel::ClickableLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent) {

     connect(this, SIGNAL(clicked()),this, SLOT(slotClicked()));
}

ClickableLabel::~ClickableLabel() {}

void ClickableLabel::mousePressEvent(QMouseEvent* event) {
    emit clicked();
}

void ClickableLabel::slotClicked()
{
   Repository* repository = new Repository();
   Image* img = repository->getImages()[0];

   QPixmap pixmap(img->fileName);
   this->setPixmap(pixmap);
   this->setScaledContents( true );
   this->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
   update();
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


