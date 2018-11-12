#ifndef CINDATABASE_H
#define CINDATABASE_H

#include "CinDBReader.h"
#include <QSqlDatabase>
#include <QString>

//! A class that manages the in-memory database created from a Cinema Database
/*!
    This class contains everything it needs to read a Cinema Database, retain
    information about the Cinema Database, and an implementation of a database
    that holds the in-memory database. 
*/

class CinDatabase 
{
    public:
        CinDatabase();
        inline void           setTableName(const QString &name) {mTableName = name;}
        inline const QString &getTableName() {return mTableName;}
        const QString        &getPath();

        int loadDatabase(QString &dbPath);

    private:
        QSqlDatabase mDatabase;
        CinDBReader  mReader;
        QString      mTableName;
};

#endif // CINDATABASE_H
