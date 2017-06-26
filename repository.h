#ifndef REPOSITORY_H
#define REPOSITORY_H
#include <list>
#include <vector>
#include <sqlite3.h>
#include "image.h"

class Repository
{
private:
    sqlite3* mDB;
    QString mImageLocationDir;
public:
    Repository();
    ~Repository();
    std::vector<Image*> getImages();
};

#endif // REPOSITORY_H
