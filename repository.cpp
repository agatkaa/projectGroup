#include "repository.h"
#include <QDebug>
#include <QSettings>
#include <QString>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

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

std::vector<Image*> Repository::getImages()
{
    std::vector<Image*> images;

    QSqlQuery query(mDB);
    if (query.exec("SELECT id, path FROM Images"))
    {
        while(query.next()) {
            Image* image = new Image();
            image->id = query.value(0).toInt();
            image->fileName = mImageLocationDir + query.value(1).toString();
            images.push_back(image);
        }
    }
    return images;
}
