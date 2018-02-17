#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QLabel>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QFileDialog>
#include "tablica.h"
#include "clickablelabel.h"
#include "repository.h"
#include <QDrag>
#include <QDebug>

Q_DECLARE_METATYPE(Image*)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // umożliwienia przesuwania obrazków z listy
    ui->listWidget->setDragEnabled(true);

    // stworzenie nowej tablicy przechowującej labele (kwadratu)
    // tworzony jest obiekt dziedziczący po QFrame, w którym rysowane są kwadraty
    // obiekt tablica umożliwia dodanie i usunięcie kwadrtów

    // !!!do przełączania między tablicami najlepiej stworzyć listę wskźników na tablice i zamiast odwoływać się do tablic odwoływać się do pozycji w liście
    tablica = new Tablica (this, &repository);
    iteratorTablic = 0;
    tablice.push_back(tablica);


    repository.syncImages();
    QList<Image*> images = repository.getImages();

    // ustawienie położenia i wielkości okna do rysowania


    // dodanie panel z zawartoscia folderu assets, wszystkie obrazy z folderu assets tam sa , mozna je przciagac za pomoca drag&drop
    for (int i=0;i<images.size();i++){
        QListWidgetItem* item = new QListWidgetItem(images.at(i)->fileName);
        item->setIcon(QIcon(images.at(i)->fullFileName));
        item->setData(Qt::UserRole, qVariantFromValue(images.at(i)));
        ui->listWidget->addItem(item);
    }
    //koniec tego panelu z zawartoscia folderu assets


    update();
}

//void MainWindow::moveButton(QPushButton *button, int x, int y)
//{
//    // aktualna pozycja
//    QPoint position = button->pos();

//    // przsunięcie przycisku
//    QPoint newPosition(position.x() + x, position.y() + y);

//    // nowa pozycja
//    button->setGeometry(QRect(newPosition.x(),newPosition.y(),41,41));
//}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addRowButton_pressed()
{

    int row = tablice.at(iteratorTablic)->table.rowNumber++;
    for (int i=0; i < tablice.at(iteratorTablic)->table.colNumber; i++)
    {
        tablice.at(iteratorTablic)->addLabel(tablice.at(iteratorTablic)->squareNumber,i,row);
    }

    currentTablica()->saveTable();
    update();
}

void MainWindow::on_addColButton_clicked()
{
    tablice.at(iteratorTablic)->table.colNumber++;
    int col = tablice.at(iteratorTablic)->table.colNumber;

    // dodanie labeli w ostatniej kolumnie
    for (int i = 0; i < tablice.at(iteratorTablic)->table.rowNumber; i++)
    {
        // dodanie labela do listy
        tablice.at(iteratorTablic)->labelList.insert(col-1+i*col, new ClickableLabel(tablice.at(iteratorTablic)));
        // dodanie nowego labela do gui
        tablice.at(iteratorTablic)->addLabel(col-1+i*col,col-1,i);
    }
    currentTablica()->saveTable();
    // wyrysowanie ekranu
    update();
}

void MainWindow::on_deleteRowButton_pressed()
{
    // usunięcie kwadratów z ostatniego wiersza (ostatnich labeli w liście)
    if(tablice.at(iteratorTablic)->table.rowNumber > 1)
    {
        for (int i = 0; i < tablice.at(iteratorTablic)->table.colNumber; i++)
        {
            tablice.at(iteratorTablic)->squareNumber--;
            tablice.at(iteratorTablic)->deleteLabel(tablice.at(iteratorTablic)->squareNumber);
        }
        tablice.at(iteratorTablic)->table.rowNumber--;
        currentTablica()->saveTable();
        update();
    }
}

void MainWindow::on_deleteColButton_pressed()
{
    // usunięcie kwadratów z ostatniej kolumny
    if(tablice.at(iteratorTablic)->table.colNumber > 1)
    {
        for (int i = 0; i < tablice.at(iteratorTablic)->table.rowNumber; i++)
        {
            // dla i = 0 ostatni element w liście
            tablice.at(iteratorTablic)->deleteLabel((tablice.at(iteratorTablic)->squareNumber - 1) - i * tablice.at(iteratorTablic)->table.colNumber);
        }
        tablice.at(iteratorTablic)->squareNumber -= tablice.at(iteratorTablic)->table.rowNumber;
        tablice.at(iteratorTablic)->table.colNumber--;
        currentTablica()->saveTable();
        update();
    }
}

void MainWindow::on_move_right_button_pressed()
{
    tablice.at(iteratorTablic)->hide();
    iteratorTablic=iteratorTablic+1;
    update();

}

void MainWindow::on_move_left_button_pressed()
{
    tablice.at(iteratorTablic)->hide();
    iteratorTablic = iteratorTablic-1;
    update();
}

void MainWindow::on_dodajTablice_pressed(){
    tablica = new Tablica (this, &repository);
    tablice.push_back(tablica);
    tablice.at(iteratorTablic)->hide();
    iteratorTablic = tablice.size()-1;
    update();
}

void MainWindow::on_pictureRead_pressed(){
    QString sourceFile = openFile();
    QFileInfo QFileinfo(sourceFile);

    QPixmap pixmap(sourceFile);
    QSettings settings(QString(":/config.ini"), QSettings::IniFormat);
    QString outputFile = settings.value("db/image_location_dir").toString()+"/"+QFileinfo.fileName();
    bool retval = QFile::copy(sourceFile, outputFile);

    tablice.at(0)->labelList.at(3)->setPixmap(pixmap);
    update();
}

// funkcja rysująca kwadraty, wywoływana przez update()
void MainWindow::paintEvent(QPaintEvent *e)
{
    if (iteratorTablic==0) {
      ui->move_left_button->setDisabled(true);
    } else ui->move_left_button->setEnabled(true);

    if (iteratorTablic==tablice.size()-1) {
      ui->move_right_button->setDisabled(true);
    }
    else {
         ui->move_right_button->setEnabled(true);
    }
    tablice.at(iteratorTablic)->setGeometry(100,20,500,500);
    tablice.at(iteratorTablic)->show();
    // pobranie wielkości okna w którym rysujemy oraz ustawienie przerwy pomiędzy kwadratami
    int width = tablice.at(iteratorTablic)->width();
    int height = tablice.at(iteratorTablic)->height();
    int margin = 10;

    int labelMaksWidth = width/tablice.at(iteratorTablic)->table.rowNumber;
    int labelMaksHeight = height/tablice.at(iteratorTablic)->table.colNumber;

    // znalezienie największego boku kwadratu mieszczącego się w oknie do rysowania
    int labelSize;
    if (labelMaksWidth < labelMaksHeight)
    {
        labelSize = labelMaksWidth;
    }else
    {
        labelSize = labelMaksHeight;
    }

    // wyrysowanie wszystkich kwadratów z listy
    for (int i = 0; i < tablice.at(iteratorTablic)->labelList.length(); i++)
    {
        int row = i/tablice.at(iteratorTablic)->table.colNumber;
        int col = i%tablice.at(iteratorTablic)->table.colNumber;
        tablice.at(iteratorTablic)->labelList.at(i)->setGeometry(QRect(col*(labelSize),row*(labelSize),labelSize-margin,labelSize-margin));
        tablice.at(iteratorTablic)->labelList.at(i)->show();
    }

    repository.syncImages();
    QList<Image*> images = repository.getImages();

    // ustawienie położenia i wielkości okna do rysowania

    ui->listWidget->clear();
    for (int i=0;i<images.size();i++){
        QListWidgetItem* item = new QListWidgetItem(images.at(i)->fileName);
        item->setIcon(QIcon(images.at(i)->fullFileName));
        item->setData(Qt::UserRole, qVariantFromValue(images.at(i)));

        ui->listWidget->addItem(item);
    }
    //koniec tego panelu z zawartoscia folderu assets

}


 QString MainWindow::openFile()
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

 // akcja dla drag and drop obrazków w listy obrazków
 void MainWindow::on_listWidget_itemPressed(QListWidgetItem *item)
 {
     QDrag *drag = new QDrag(item->listWidget());   // utworzenie obiektu drag dla obrazka z listy
     QMimeData *mime= new QMimeData;    // obiekt przechwoujący dane dla drag and drop

     Image* image = (Image*) item->data(Qt::UserRole).value<Image*>();

     QString string (image->fullFileName);
     mime->setText(string); // przypisanie nazwy obrazka do danych
     drag->setMimeData(mime);   // dodanie danych do obiektu drag

     // ustawienie i umiejscowienia ikony pojawiającej się przy akcji drag and drop
     QIcon qicon = item->icon();
     QPixmap pixmap = qicon.pixmap(24,24);
     drag->setPixmap(pixmap);
     drag->setHotSpot(QPoint(15,15));

     drag->exec();  //  rozpoczęcie akcji drag and drop
 }

 Tablica* MainWindow::currentTablica()
 {
     return tablice.at(iteratorTablic);
 }
