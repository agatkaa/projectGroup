#include "repository.h"
#include <QDebug>
#include <QSettings>
#include <QString>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>

Repository::Repository()
{
    QSettings settings(QString(":/config.ini"), QSettings::IniFormat);
    QString DBFilePath = settings.value("db/file_location").toString();
    mImageLocationDir = settings.value("db/image_location_dir").toString();
    if (!QFile::exists(DBFilePath)) {
        qDebug() << "DB file does not exists: " << DBFilePath;
    }
    if (!QDir(mImageLocationDir).exists()) {
        qDebug() << "Image location directory does not exists: " << mImageLocationDir;
    }
    mDB = QSqlDatabase::addDatabase("QSQLITE");
    mDB.setDatabaseName(DBFilePath);
    if (!mDB.open())
    {
        qDebug() << "Error: connection with database fail: " << mDB.lastError() << " file: " << DBFilePath;
    }
}

Repository::~Repository()
{
    mDB.close();
}

QList<Image*> Repository::getImages()
{
    QList<Image*> images;

    QSqlQuery query(mDB);
    if (query.exec("SELECT id, path FROM Images"))
    {
        while(query.next()) {
            QString path = query.value(1).toString();
            Image* image = new Image();
            image->id = query.value(0).toInt();
            image->fileName = path;
            image->fullFileName = mImageLocationDir + path;
            images.push_back(image);
        }
    }
    else {
        qDebug() << "error while loading images: " << query.lastError();
    }
    return images;
}

void Repository::syncImages()
{
    //obrazki w bazie
    QList<Image*> currentImages = getImages();
    QSet<QString> currentImagesNames;
    for(int i = 0; i < currentImages.size(); ++i)
    {
        currentImagesNames << currentImages[i]->fileName;
    }

    //obrazki w pliku
    QSet<QString> images = getImagesNamesFromImageLocation();

    //obrazki ktore sa w pliku ale nie ma ich w bazie
    QSet<QString> imagesToAdd = QSet<QString>(images).subtract(currentImagesNames);
    addImages(imagesToAdd);

    //obrazki ktore sa w bazie ale nie ma ich w pliku
    QSet<QString> imagesToRemove = QSet<QString>(currentImagesNames).subtract(images);
    removeImages(imagesToRemove);

    for(int i = 0; i < currentImages.size(); ++i) {
        delete currentImages.at(i);
    }
}

QSet<QString> Repository::getImagesNamesFromImageLocation()
{
    QDir folder(mImageLocationDir);
    folder.setNameFilters(QStringList()<<"*.png");
    QStringList fileList = folder.entryList();

    QSet<QString> imageNames;
    for(int i = 0; i < fileList.size(); ++i)
    {
        imageNames << fileList.at(i);
    }
    return imageNames;
}

void Repository::addImages(QSet<QString>& imagesNames)
{
    QSqlQuery query;
    query.prepare("INSERT INTO images(path, popularity) VALUES (:path, :popularity) ");
    for (QSet<QString>::iterator it = imagesNames.begin(); it != imagesNames.end(); ++it)
    {
        query.bindValue(":path", *it);
        query.bindValue(":popularity", 0);
        if (query.exec()) {
            qDebug() << "added image: " << *it;
        }
        else {
            qDebug() << "error: " << query.lastError() << " while adding image: " << *it;
        }
    }
}

void Repository::removeImages(QSet<QString>& imagesNames)
{
    QSqlQuery query;
    query.prepare("DELETE FROM images WHERE path = :path ");
    for (QSet<QString>::iterator it = imagesNames.begin(); it != imagesNames.end(); ++it)
    {
        query.bindValue(":path", *it);
        if (query.exec()) {
            qDebug() << "removed image: " << *it;
        }
        else {
            qDebug() << "error: " << query.lastError() << " while removing image: " << *it;
        }
    }
}

int Repository::saveTable(Table &table)
{
    int id;
    if (table.id > 0)
    {
        id = updateTable(table);
    }
    else
    {
        id = addTable(table);
    }
    return id;
}

Table Repository::getTable(int id)
{
    Table table;
    if (id < 0) {
        qDebug() << "wrong table id " << id;
    }
    QSqlQuery query;
    query.prepare("SELECT id, name, rowsNumber, colsNumber, lastUpdate FROM Tables WHERE id = :id");
    query.bindValue(":id", id);
    if (query.exec()) {
        if (query.next()) {
            table.id = query.value(0).toInt();
            table.name = query.value(1).toString();
            table.rowNumber = query.value(2).toInt();
            table.colNumber = query.value(3).toInt();
            table.lastUpdate = query.value(4).toDate();
        }
        else {
            qDebug() << "nie znaleziono tablicy z id " << id << " w bazie danych";
        }
    }
    else {
        qDebug() << "error while getting table: " << id << ", " << query.lastError();
    }
    return table;
}

int Repository::updateTable(Table &table)
{
    Table current = getTable(table.id);
    if (current.lastUpdate != table.lastUpdate) {
        qDebug() << "cannot update table, someone modified it before you ";
        return -1;
    }
    QSqlQuery query;
    query.prepare("UPDATE Tables "
                  "set name = :name "
                  ", rowsNumber = :rowNumber "
                  ", colsNumber = :colNumber "
                  ", lastUpdate = :lastUpdate "
                  "where id = :id ");
    query.bindValue(":name", table.name);
    query.bindValue(":rowNumber", table.rowNumber);
    query.bindValue(":colNumber", table.colNumber);
    query.bindValue(":lastUpdate", QDate::currentDate());
    query.bindValue(":id", table.id);
    if (query.exec())
    {
        qDebug() << "updated table " << table.displayName();
        return table.id;
    }
    else
    {
        qDebug() << "error while updating table " << table.displayName()<< ", " << query.lastError();
        return -1;
    }
}

int Repository::saveImageInTable(int id, QString path, int x, int y)
{
    Table current = getTable(id);
    if (current.lastUpdate != current.lastUpdate) {
        qDebug() << "cannot update table, someone modified it before you ";
        return -1;
    }

    int imageInTable_id = checkImageInTable(current, x,y);
    int image_id = getImageIdFromPath(path);

    if (imageInTable_id!=-1){
        QSqlQuery query;
        query.prepare("UPDATE ImageInTable "
                      "set Images_id = :Images_id "
                      "where id=:id");
        query.bindValue(":Images_id", image_id);
        query.bindValue(":id", imageInTable_id);

        if (query.exec())
        {
            qDebug() << "updated table " << current.displayName();
            query.prepare("UPDATE Tables "
                          "set lastUpdate = :lastUpdate "
                          "where id = :id ");
            query.bindValue(":lastUpdate", QDate::currentDate());
            query.bindValue(":id", current.id);

            if (query.exec())
            {
                qDebug() << "updated table " << current.displayName();
                return current.id;
            }
            else
            {
                qDebug() << "error while updating table " << current.displayName()<< ", " << query.lastError();
                return -1;
            }
            return current.id;
        }
        else
        {
            qDebug() << "error while updating table " << current.displayName()<< ", " << query.lastError();
            return -1;
        }


    }
    else {
        QSqlQuery query;
        query.prepare("INSERT INTO ImageInTable(Tables_id, Images_id, xPosition, yPosition)"
                      " VALUES (:Tables_id, :Images_id, :xPosition, :yPosition)");
        query.bindValue(":Tables_id", current.id);
        query.bindValue(":Images_id", image_id);
        query.bindValue(":xPosition", x);
        query.bindValue(":yPosition", y);

        if (query.exec())
        {
            qDebug() << "added location of image in table " << current.displayName();
            return current.id;
        }
        else
        {
            qDebug() << "error while adding image location in table " << current.displayName()<< ", " << query.lastError();
            return -1;
        }
        return current.id;

    }
}

int Repository::addTable(Table &table)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Tables(name, rowsNumber, colsNumber) VALUES (:name, :rowNumber, :colNumber)");
    query.bindValue(":name", table.name);
    query.bindValue(":rowNumber", table.rowNumber);
    query.bindValue(":colNumber", table.colNumber);
    if (query.exec())
    {
        int id = query.lastInsertId().toInt();
        table.id = id;
        qDebug() << "added table " << table.displayName();
        return id;
    }
    else
    {
        qDebug() << "Error while adding table " << table.displayName() << ", " << query.lastError();
        return -1;
    }
}

int Repository::checkImageInTable(Table &table,int x, int y){

    QSqlQuery query;
    query.prepare("SELECT id,Images_id FROM ImageInTable WHERE Tables_id=:tableId AND xPosition=:x AND yPosition=:y");
    query.bindValue(":tableId", table.id);
    query.bindValue(":x", x);
    query.bindValue(":y", y);
    if (query.exec()) {
        if (query.next()) {
            return query.value(0).toInt();
        }
        else {
            return -1;
            qDebug() << "nie znaleziono zapisu dla tej pozycji w tablicy:  " << table.id << " w bazie danych";
        }
    }
    else {
        qDebug() << "error while getting table: " << ", " << query.lastError();
    }
}

int Repository::getImageIdFromPath(QString path){

    QSqlQuery query;
    query.prepare("SELECT id FROM Images WHERE path=:path");
    query.bindValue(":path", path);
    if (query.exec()) {
        if (query.next()) {
            return query.value(0).toInt();
        }
        else {
            return -1;
            qDebug() << "nie znaleziono takiego obrazka w bazie danych";
        }
    }
    else {
        qDebug() << "error while getting image: " << ", " << query.lastError();
    }
}
