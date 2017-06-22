#ifndef REPOSITORY_H
#define REPOSITORY_H
#include <list>
#include <vector>
#include "image.h"

class Repository
{
public:
    Repository();
    std::vector<Image*> getImages();
};

#endif // REPOSITORY_H
