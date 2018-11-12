#include "CinDatabase.h"
#include "CinDBReader.h"
#include <QSqlDatabase>
#include <QFileInfo>


CinDatabase::CinDatabase() 
{
    // database
    mDatabase = QSqlDatabase::addDatabase("QSQLITE");
    mDatabase.open();

    // other
    mTableName = "cinema";
}

int CinDatabase::loadDatabase(QString &dbPath)
{
    int result = CinDBReader::DatabaseLoadError; 

    if ( ! (QFileInfo::exists(dbPath) && QFileInfo(dbPath).isDir()) )
    {
        result = mReader.readCinemaDatabase(this->mDatabase, dbPath, this->mTableName);
    }

    return result;
}

const QString &CinDatabase::getPath()
{
    return mReader.getCurDatabase();
}

