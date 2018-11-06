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

        int         type=CinDBColData::UNDEFINED;
        // float       min=0.0;
        // float       max=0.0;
        // bool        rangeInitialized=false;
        std::string name;

};

class CinDBReader
{
    public:
        CinDBReader();
        int init();
        int readCSV(QSqlDatabase &db, const char *path);

    private:
        CinDBColData::Type getType(QString &value);
        void split(const std::string & s, char c, std::vector<std::string>& v);
        // void adjustRange(CinDBColData &c, float value);
        int  loadDB(QSqlDatabase &db, const char *path, std::vector<CinDBColData> &coldata);
        void constructCommands(const char *dbname, std::vector<CinDBColData> &coldata, QString &create, QString &insert);
};

#endif // CINDBREADER_H
