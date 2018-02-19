#ifndef REPOSITORY_H
#define REPOSITORY_H
#include <list>
#include <vector>
#include <QSqlDatabase>
#include "image.h"
#include "table.h"

class Repository
{
private:
    QSqlDatabase mDB;
    QString mImageLocationDir;
    //lista z nazwami obrazkow
    QSet<QString> getImagesNamesFromImageLocation();
    void addImages(QSet<QString>& imagesNames);
    void removeImages(QSet<QString>& imagesNames);

    int updateTable(Table& table);
    int addTable(Table& table);

    int checkImageInTable(Table &table, int x, int y);
    int getImageIdFromPath(QString path);
public:
    Repository();
    ~Repository();
    //pobranie wszystkich obrazkow z bazy
    QList<Image*> getImages();

    //synchronizacja obrazkow w bazie z folderem, do bazy sa obrazki ktorych jeszcze tam nie bylo
    void syncImages();
int saveImageInTable(int id, QString path, int x, int y);
    int saveTable(Table& table);
    Table getTable(int id);
};

#endif // REPOSITORY_H
