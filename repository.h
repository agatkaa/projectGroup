#ifndef REPOSITORY_H
#define REPOSITORY_H
#include <list>
#include "image.h"

class Repository
{
public:
    Repository();
    std::list<Image*> getImages();
};

#endif // REPOSITORY_H
