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
        int readCinemaDatabase(QSqlDatabase &db, const char *path);

    private:
        DBColData::Type getType(QString &value);
        void split(const std::string & s, char c, std::vector<std::string>& v);
        int  loadDB(QSqlDatabase &db, const char *path, std::vector<DBColData> &coldata);
        void constructCommands(const char *dbname, std::vector<DBColData> &coldata, QString &create, QString &insert);
        void constructNewTableCommand(QString &newTableCommand, const char *initTable, const char *finalTable);
        void readSettings(const char *path);

        std::vector<QString> mColOrder;
};

}

#endif // CINDBREADER_H
