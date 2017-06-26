#include "repository.h"
#include <sqlite3.h>
#include <QDebug>
#include <QSettings>
#include <QString>
#include <QDir>
//sudo apt-get install sqlite3
//sudo apt-get install libsqlite3-dev

Repository::Repository()
{
    QSettings settings(QString(":/config.ini"), QSettings::IniFormat);
    QString DBFilePath = settings.value("db/file_location").toString();
    mImageLocationDir = settings.value("db/image_location_dir").toString();
    if (!QDir(mImageLocationDir).exists()) {
        qDebug() << "Image location directory does not exists: " << mImageLocationDir;
    }

    int ret;
    if (SQLITE_OK != (ret = sqlite3_initialize()))
    {
        qDebug() << "Failed to initialize library: " << ret;
    }
    if (SQLITE_OK != (ret = sqlite3_open_v2(DBFilePath.toStdString().c_str(), &mDB, SQLITE_OPEN_READWRITE, NULL)))
    {
        qDebug() << "Failed to open conn: " << ret << ", error: " << sqlite3_errmsg(mDB) << ", db_file: " << DBFilePath;
    }
}

Repository::~Repository()
{
    if (mDB != NULL)
    {
        sqlite3_close(mDB);
    }
    sqlite3_shutdown();
}

std::vector<Image*> Repository::getImages()
{
    std::vector<Image*> images;
    sqlite3_stmt* stmt;
    int res = sqlite3_prepare_v2(mDB, "select id, path from Images;", -1, &stmt, NULL);
    if (res != SQLITE_OK) {
        qDebug() << "Failed to prepare statement:" << res << "error: " << sqlite3_errmsg(mDB);
    }
    while (SQLITE_ROW == (res = sqlite3_step( stmt ))) {
        Image* image = new Image();
        image->id = sqlite3_column_int(stmt, 0);
        image->fileName = mImageLocationDir + reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        images.push_back(image);
    }
    if (SQLITE_DONE != res) {
        qDebug() << "Failed while iterating over results: " << res;
    }
    if (stmt != NULL) {
        sqlite3_finalize(stmt);
    }
    return images;
}
