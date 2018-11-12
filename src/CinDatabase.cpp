#include "CinDatabase.h"
#include "CinDBReader.h"
#include <QSqlDatabase>
#include <QFileInfo>

using namespace cin;

CDatabase::CDatabase() 
{
    // database
    mDatabase = QSqlDatabase::addDatabase("QSQLITE");
    mDatabase.open();

    // other
    mTableName = "cinema";
}

int CDatabase::loadDatabase(QString &dbPath)
{
    int result = DBReader::DatabaseLoadError; 

    if ( ! (QFileInfo::exists(dbPath) && QFileInfo(dbPath).isDir()) )
    {
        result = mReader.readCinemaDatabase(this->mDatabase, dbPath, this->mTableName);
    }

    return result;
}

const QString &CDatabase::getPath()
{
    return mReader.getCurDatabase();
}

