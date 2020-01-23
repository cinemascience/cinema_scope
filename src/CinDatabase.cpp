#include "CinDatabase.h"
#include "CinDBReader.h"
#include <QDebug>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QRegExp>
#include <QRandomGenerator>
// #include <QVariant>



// class statics
QString CinDatabase::DefaultTableName   = "cinema";


/*! \brief Constructor sets up database back end 
 *
 */
CinDatabase::CinDatabase() 
{
    // database
        // TODO: determine if we need to change name of database 
        // using this string amounts to hard coding 
        // a datbase into this class
    QString connect;
    this->getUniqueConnectionName(connect);
    mDatabase = QSqlDatabase::addDatabase("QSQLITE", connect);
    mDatabase.open();

    // other
    mTableName = CinDatabase::DefaultTableName; 
}

CinDatabase::~CinDatabase() 
{
    // this appears to be a way of removing this connection name from
    // the Qt infrasctructure. The old mDatabase object holds a 
    // reference to the connection, so it must be set to a new
    // database object before the connection is destroyed, or
    // the removeDatabase call will fail
    QString connection = mDatabase.connectionName();
    mDatabase = QSqlDatabase();
    mDatabase.removeDatabase(connection);
}

/*! \brief Load Cinema database from disk into this object 
 *
 */
int CinDatabase::load(const QString &dbPath)
{
    int result = CinDBReader::DatabaseLoadError; 
    qDebug() << "CINDATABASE LOAD:" << dbPath; 

    if (QFileInfo::exists(dbPath) && QFileInfo(dbPath).isDir())
    {
        result = mReader.load(mDatabase, dbPath, mTableName);
        setColumnNames();
    }
    // debugdump();

    return result;
}

const QString &CinDatabase::getPath()
{
    return mReader.getDatabase();
}

/*! \brief Save a local list of column names from the database 
 *
 */
void CinDatabase::setColumnNames()
{
    QSqlRecord record = mDatabase.record(getTableName());

    QString column;
    for (int i=0;i<record.count();i++)
    {
        column = record.field(i).name();
        // qDebug() << "COLUMN: " << column;
        // QVariant variant = record.field(i).type();
        // qDebug() << "COLUMN: " << column << ", TYPE: " << variant.typeName();
        if (not isArtifactColumn(column) ) {
            mParameterColNames.push_back(column);
        } else {
            mArtifactColNames.push_back(column);
        }
    }
}

/*! \brief Determine if a name is an artifact column name
 *  
 *  Matched per the specification 
 */
bool CinDatabase::isArtifactColumn(const QString &name)
{
    QRegExp rx("^FILE.*");

    return rx.exactMatch(name);
}

void CinDatabase::getUniqueConnectionName(QString &connect)
{
    connect = "connect:" + QString::number(QRandomGenerator::global()->generateDouble());
}

void CinDatabase::reset()
{
    QSqlQuery query(mDatabase);
    query.exec("DROP TABLE " + mTableName);

    mParameterColNames.clear();
    mArtifactColNames.clear();
}

/*! \brief Perform a specific data dump for debugging
 *
 *  This is intended to be hard coded for debugging - it's
 *  a convenient wrapper to call ...
 */
void CinDatabase::debugdump()
{
    QSqlQuery query(mDatabase);
    query.exec("SELECT [timestep] from cinema"); 

    while (query.next())
    {
        qDebug() << "value: " << query.value(0).toString();
    }
}
