#ifndef CINDATABASE_H
#define CINDATABASE_H

#include "CinDBReader.h"
#include <QSqlDatabase>
#include <QString>
#include <QStringList>

//! Manages the in-memory database created from a Cinema Database
/*!
 *  This class contains everything it needs to read a Cinema Database, retain
 *  information about the Cinema Database, and an implementation of a database
 *  that holds the in-memory database. 
*/
class CinDatabase 
{
    public:
        CinDatabase();
        int load(const QString &dbPath);
        inline const QString &getTableName() {return mTableName;}

        void                  setParameterColumnNames();
        const QStringList    &getParameterColumnNames() {return mParameterColumnNames;}
        int                   getNumParameterColumns();
        const QString        &getPath();
        const QString        &getArtifactColumnName() {return CinDatabase::ArtifactColumnName;}


    private:
        bool isArtifactColumn(const QString &name);
        inline void setTableName(const QString &name) {mTableName = name;}

        QSqlDatabase mDatabase;     /*!< The Sql back end for this database object */
        CinDBReader  mReader;       /*!< The reader object for the database on disk */ 
        QString      mTableName;    /*!< The name of the table created */
        QStringList  mParameterColumnNames;  /*!< List of non-artifact column names */

        static QString ArtifactColumnName;/*!< Name of the column that holds the artifacts */
        static QString DefaultTableName;  /*!< Name of the column that holds the artifacts */
};

#endif // CINDATABASE_H
