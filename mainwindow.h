#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsScene>
#include "tablica.h"
#include <QListWidgetItem>
#include "repository.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent *e);

private slots:
    void on_addRowButton_pressed();

    void on_addColButton_clicked();

    void on_deleteRowButton_pressed();

    void on_deleteColButton_pressed();

    void on_move_right_button_pressed();

    void on_move_left_button_pressed();

    void on_dodajTablice_pressed();

    void on_pictureRead_pressed();

    QString openFile();

    void on_listWidget_itemPressed(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    QLabel *label;
    QPushButton *button1;
    QGraphicsScene *scene;

    Tablica *tablica;
    Tablica *tablica2;
    QList<Tablica*> tablice;
    int iteratorTablic;
    Repository repository;
    void moveButton (QPushButton *button, int x, int y);

    Tablica* currentTablica();
};

#endif // MAINWINDOW_H
