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
        float       min=0.0;
        float       max=0.0;
        bool        rangeInitialized=false;
        std::string name;

};

class CinDBReader
{
    private:
        void split(const std::string & s, char c, std::vector<std::string>& v);
        bool isInt(const std::string &s);
        bool isFloat(const std::string &s);
        void adjustRange(CinDBColData &c, float value);

    public:
        CinDBReader();
        int init();
        int readCSV(QSqlDatabase &db, const char *path);
};

#endif // CINDBREADER_H
