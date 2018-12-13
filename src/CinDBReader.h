#ifndef CINDBREADER_H
#define CINDBREADER_H

#include <QSqlDatabase>
#include <string>
#include <vector>

//! Helper class for CinDBReader 
/*!
*/
class CinDBColData
{
    public:
        CinDBColData() {}

        enum Type{UNDEFINED=0, STRING, FLOAT, INT};

        int type=CinDBColData::UNDEFINED;
        std::string name;

};

//! Reads a Cinema database from disk into an in-memory database 
/*!
 *  This class reads a cinema database from disk and places the data
 *  in a QSqlDatabase. Possible operations on the database include
 *  filtering (via data stored in a file within the cinema database).
 *
 *  Filtering is accomplished by reading all data from disk into an
 *  initial table,  then copying the filtered data into a final table.
 *  The name of the final table is provided to this object (see public
 *  functions.
 *
 *  The class could read data into many types of in-memory databases,
 *  as it does not own the implementation of the in-memory database.
 *  Instead, the database is handed to this class during the read
 *  operation.
*/
class CinDBReader
{
    public:
        CinDBReader();
        int load(QSqlDatabase &db, const QString &path, const QString &tableName);

        static int VerifyDatabase(const QString &path);
        enum Results{DatabaseLoaded=1, DatabaseInvalid, DatabaseLoadError};
        static const QString NOFILE;

        const QString &getDatabase()     {return mCurDatabase;}
        const QString &getCSVFile()      {return mCurCSVFile;}
        const QString &getSettingsFile() {return mCurSettingsFile;}

        bool hasCSVFile() {return mCurCSVFile != CinDBReader::NOFILE;}
        bool hasSettingsFile() {return mCurSettingsFile != CinDBReader::NOFILE;}

    private:
        CinDBColData::Type getType(QString &value);
        void split(const std::string & s, char c, std::vector<std::string>& v);
        void loadDB(QSqlDatabase &db, const QString &tableName, std::vector<CinDBColData> &coldata);
        void constructCommands(const QString &tableName, std::vector<CinDBColData> &coldata, QString &create);
        void constructNewTableCommand(QString &newTableCommand, const QString &initTable, const QString &finalTable);
        void readSettings();
        void setCurDatabase(const QString &path);

        // instance variables
        std::vector<QString> mColOrder;
        QString mCurDatabase;
        QString mCurCSVFile;
        QString mCurSettingsFile;

        // Class variables
        static const QString CSVFile;
        static const QString SettingsFile;
        static const QString InitTableName;
};

#endif // CINDBREADER_H
