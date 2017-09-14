#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QLabel>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QFileDialog>
#include "tablica.h"
#include <clickablelabel.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // stworzenie nowej tablicy przechowującej labele (kwadratu)
    // tworzony jest obiekt dziedziczący po QFrame, w którym rysowane są kwadraty
    // obiekt tablica umożliwia dodanie i usunięcie kwadrtów

    // !!!do przełączania między tablicami najlepiej stworzyć listę wskźników na tablice i zamiast odwoływać się do tablic odwoływać się do pozycji w liście
    tablica = new Tablica (this);
    iteratorTablic = 0;
    tablice.push_back(tablica);

     //fragment z dodaniem listy z obrazkami z folderu
    QSettings settings(QString(":/config.ini"), QSettings::IniFormat);
    QDir export_folder(settings.value("db/image_location_dir").toString());
    export_folder.setNameFilters(QStringList()<<"*.png");
    QStringList fileList = export_folder.entryList();

    // ustawienie położenia i wielkości okna do rysowania
     QString name;

    QVector <QListWidgetItem*> items;
    for (int i=0;i<fileList.size();i++){
        name = fileList.at(i);
        items.push_back(new QListWidgetItem(name));
        items[i]->setIcon(QIcon(settings.value("db/image_location_dir").toString()+name));
        ui->listWidget->addItem(items[i]);
    }
    //koniec tego panelu z zawartoscia folderu assets

    update();
}

void MainWindow::moveButton(QPushButton *button, int x, int y)
{
    // aktualna pozycja
    QPoint position = button->pos();

    // przsunięcie przycisku
    QPoint newPosition(position.x() + x, position.y() + y);

    // nowa pozycja
    button->setGeometry(QRect(newPosition.x(),newPosition.y(),41,41));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addRowButton_pressed()
{

    for (int i=0; i < tablice.at(iteratorTablic)->colNumber; i++)
    {
        tablice.at(iteratorTablic)->addLabel(tablice.at(iteratorTablic)->squareNumber);
    }
    tablice.at(iteratorTablic)->rowNumber++;
    update();
}

void MainWindow::on_addColButton_clicked()
{
    tablice.at(iteratorTablic)->colNumber++;

    // dodanie labeli w ostatniej kolumnie
    for (int i = 0; i < tablice.at(iteratorTablic)->rowNumber; i++)
    {
        // dodanie labela do listy
        tablice.at(iteratorTablic)->labelList.insert(tablice.at(iteratorTablic)->colNumber-1+i*tablice.at(iteratorTablic)->colNumber, new ClickableLabel(tablice.at(iteratorTablic)));
        // dodanie nowego labela do gui
        tablice.at(iteratorTablic)->addLabel(tablice.at(iteratorTablic)->colNumber-1+i*tablice.at(iteratorTablic)->colNumber);
    }
    // wyrysowanie ekranu
    update();
}

void MainWindow::on_deleteRowButton_pressed()
{
    // usunięcie kwadratów z ostatniego wiersza (ostatnich labeli w liście)
    if(tablice.at(iteratorTablic)->rowNumber > 1)
    {
        for (int i = 0; i < tablice.at(iteratorTablic)->colNumber; i++)
        {
            tablice.at(iteratorTablic)->squareNumber--;
            tablice.at(iteratorTablic)->deleteLabel(tablice.at(iteratorTablic)->squareNumber);
        }
        tablice.at(iteratorTablic)->rowNumber--;
        update();
    }
}

void MainWindow::on_deleteColButton_pressed()
{
    // usunięcie kwadratów z ostatniej kolumny
    if(tablice.at(iteratorTablic)->colNumber > 1)
    {
        for (int i = 0; i < tablice.at(iteratorTablic)->rowNumber; i++)
        {
            // dla i = 0 ostatni element w liście
            tablice.at(iteratorTablic)->deleteLabel((tablice.at(iteratorTablic)->squareNumber - 1) - i * tablice.at(iteratorTablic)->colNumber);
        }
        tablice.at(iteratorTablic)->squareNumber -= tablice.at(iteratorTablic)->rowNumber;
        tablice.at(iteratorTablic)->colNumber--;
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
    tablica = new Tablica (this);
    tablice.push_back(tablica);
    tablice.at(iteratorTablic)->hide();
    iteratorTablic = tablice.size()-1;
    update();
}

void MainWindow::on_pictureRead_pressed(){
    QPixmap pixmap(openFile());
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

    int labelMaksWidth = width/tablice.at(iteratorTablic)->rowNumber;
    int labelMaksHeight = height/tablice.at(iteratorTablic)->colNumber;

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
        int row = i/tablice.at(iteratorTablic)->colNumber;
        int col = i%tablice.at(iteratorTablic)->colNumber;
        tablice.at(iteratorTablic)->labelList.at(i)->setGeometry(QRect(col*(labelSize),row*(labelSize),labelSize-margin,labelSize-margin));
        tablice.at(iteratorTablic)->labelList.at(i)->show();
    }


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
