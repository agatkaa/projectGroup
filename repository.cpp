#include "repository.h"

Repository::Repository()
{

}

std::list<Image*> Repository::getImages()
{
    std::list<Image*> images;
    Image* image = new Image();
    image->id = 1;
    image->fileName = "fajny.jpg";
    images.push_back(image);
    return images;
}
