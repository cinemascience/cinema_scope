#ifndef CINDBREADER_H
#define CINDBREADER_H

#include <QSqlDatabase>
#include <string>
#include <vector>

namespace cin {

class DBColData
{
    public:
        DBColData() {}

        enum Type{UNDEFINED=0, STRING, FLOAT, INT};

        int type=DBColData::UNDEFINED;
        std::string name;

};

class DBReader
{
    public:
        DBReader();
        int readCinemaDatabase(QSqlDatabase &db, const QString &path, const QString &tableName);

        enum Results{DatabaseLoaded=1, DatabaseInvalid, DatabaseLoadError};

    private:
        DBColData::Type getType(QString &value);
        void split(const std::string & s, char c, std::vector<std::string>& v);
        void loadDB(QSqlDatabase &db, const QString &path, const QString &tableName, std::vector<DBColData> &coldata);
        void constructCommands(const QString &tableName, std::vector<DBColData> &coldata, QString &create, QString &insert);
        void constructNewTableCommand(QString &newTableCommand, const QString &initTable, const QString &finalTable);
        void readSettings(QString &path);

        std::vector<QString> mColOrder;
        QString mCurDatabase;

        // Class variables
        static const QString CSVFile;
        static const QString SettingsFile;
        static const QString InitTableName;
};

}

#endif // CINDBREADER_H
