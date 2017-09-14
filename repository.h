#ifndef REPOSITORY_H
#define REPOSITORY_H
#include <list>
#include <vector>
#include <QSqlDatabase>
#include "image.h"

class Repository
{
private:
    QSqlDatabase mDB;
    QString mImageLocationDir;
    //lista z nazwami obrazkow
    QSet<QString> getImagesNamesFromImageLocation();
    void addImages(QSet<QString>& imagesNames);
    void removeImages(QSet<QString>& imagesNames);
public:
    Repository();
    ~Repository();
    //pobranie wszystkich obrazkow z bazy
    QList<Image*> getImages();

    //synchronizacja obrazkow w bazie z folderem, do bazy sa obrazki ktorych jeszcze tam nie bylo
    void syncImages();
};

#endif // REPOSITORY_H
