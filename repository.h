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
public:
    Repository();
    ~Repository();
    std::vector<Image*> getImages();
};

#endif // REPOSITORY_H
