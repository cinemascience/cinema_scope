#include "CinDatabase.h"
#include "CinDBReader.h"
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlField>
#include <QSqlRecord>



// class statics
QString CinDatabase::ArtifactColumnName = "FILE";


/*! \brief Constructor sets up database back end 
 *
 */
CinDatabase::CinDatabase() 
{
    // database
    mDatabase = QSqlDatabase::addDatabase("QSQLITE");
    mDatabase.open();

    // other
    mTableName = "cinema";
}

/*! \brief Load Cinema database from disk into this object 
 *
 */
int CinDatabase::loadDatabase(const QString &dbPath, const QString &tableName)
{
    int result = CinDBReader::DatabaseLoadError; 

    if ( ! (QFileInfo::exists(dbPath) && QFileInfo(dbPath).isDir()) )
    {
        setTableName(tableName); 
        result = mReader.readCinemaDatabase(this->mDatabase, dbPath, this->mTableName);
        setParameterColumnNames();
    }

    return result;
}

const QString &CinDatabase::getPath()
{
    return mReader.getCurDatabase();
}

/*! \brief Save a local list of column names from the database 
 *
 */
void CinDatabase::setParameterColumnNames()
{
    QSqlRecord record = mDatabase.record(getTableName());

    QString column;
    for (int i=0;i<record.count();i++)
    {
        column = record.field(i).name();
        if (not isArtifactColumn(column) ) {
            mParameterColumnNames.push_back(column);
        }
    }
}

bool CinDatabase::isArtifactColumn(const QString &name)
{
    return (name == CinDatabase::ArtifactColumnName);
}

int CinDatabase::getNumParameterColumns()
{
    return mParameterColumnNames.count();
}



