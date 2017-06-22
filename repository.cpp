#include "repository.h"

Repository::Repository()
{

}

std::vector<Image*> Repository::getImages()
{
    std::vector<Image*> images;
    Image* image = new Image();
    image->id = 1;
    image->fileName = "C:/Users/Agata/Pictures/58fe5d26aa.jpg";
    images.push_back(image);
    return images;
}
