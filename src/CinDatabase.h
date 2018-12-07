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
        ~CinDatabase();

        int                   load(const QString &dbPath);
        void                  reset();
        inline const QString &getTableName() {return mTableName;}

        const QString        &getPath();
        const QStringList    &getParameterColumnNames() {return mParameterColNames;}
        const QStringList    &getArtifactColumnNames() {return mArtifactColNames;} 
        void                  addArtifactColumnName(const QString &name);
        QSqlDatabase         &getDatabase() {return mDatabase;}


    private:
        bool        isArtifactColumn(const QString &name);
        inline void setTableName(const QString &name) {mTableName = name;}
        void        setColumnNames();
        void        getUniqueConnectionName(QString &connect);

        QSqlDatabase mDatabase;             /*!< The Sql back end for this database object */
        CinDBReader  mReader;               /*!< The reader object for the database on disk */ 
        QString      mTableName;            /*!< The name of the table created */
        QStringList  mParameterColNames;    /*!< List of non-artifact column names */
        QStringList  mArtifactColNames;     /*!< Name of the column that holds the artifacts */

        static QString DefaultTableName;    /*!< Default name of the table for this database*/
};

#endif // CINDATABASE_H
