#include "repository.h"
#include <sqlite3.h>
#include <QDebug>
#include <QSettings>
#include <QString>
//sudo apt-get install sqlite3
//sudo apt-get install libsqlite3-dev

Repository::Repository()
{
    QSettings settings(QString(":/config.ini"), QSettings::IniFormat);
    QString file_path = settings.value("db/file_location").toString();

    qDebug() << settings.fileName();
    qDebug() << file_path;
    sqlite3 *db;
    sqlite3_stmt * stmt;
    if(sqlite3_open("file_path", &db) == SQLITE_OK) {
        qDebug() << "opened";
        //int res = sqlite3_prepare_v2(db, "select path, id from Images;", -1, &stmt, NULL);
        int res = sqlite3_prepare_v2(db, "SELECT SQLITE_VERSION()", -1, &stmt, NULL);
        if (res != SQLITE_OK) {
            qDebug() << "prepare:" << res << "error:" << sqlite3_errmsg(db);
        }
        res = sqlite3_step( stmt );
        if (res == SQLITE_ROW) {
            qDebug() << "verion:" << *sqlite3_column_text(stmt, 0);
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
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
