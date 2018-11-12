#ifndef CINDBREADER_H
#define CINDBREADER_H

#include <QSqlDatabase>
#include <string>
#include <vector>

class CinDBColData
{
    public:
        CinDBColData() {}

        enum Type{UNDEFINED=0, STRING, FLOAT, INT};

        int type=CinDBColData::UNDEFINED;
        std::string name;

};

class CinDBReader
{
    public:
        CinDBReader();
        int readCinemaDatabase(QSqlDatabase &db, const QString &path, const QString &tableName);
        static int VerifyDatabase(const QString &path);

        enum Results{DatabaseLoaded=1, DatabaseInvalid, DatabaseLoadError};
        const QString &getCurDatabase() {return mCurDatabase;}
        const QString &getCurCSVFile() {return mCurCSVFile;}
        const QString &getCurSettingsFile() {return mCurSettingsFile;}

    private:
        CinDBColData::Type getType(QString &value);
        void split(const std::string & s, char c, std::vector<std::string>& v);
        void loadDB(QSqlDatabase &db, const QString &tableName, std::vector<CinDBColData> &coldata);
        void constructCommands(const QString &tableName, std::vector<CinDBColData> &coldata, QString &create, QString &insert);
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
