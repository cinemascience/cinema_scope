#include "CinDBReader.h"
#include <QSqlDatabase>
#include <fstream>
#include <iostream>
#include <QtGlobal>
#include <QtDebug>
#include <string>
#include <vector>


CinDBReader::CinDBReader() 
{
}

int CinDBReader::init()
{
    int res = 0;
    return res;
}

int CinDBReader::readCSV(QSqlDatabase &db, const char *path)
{
    int res = 0;

    std::ifstream input(path);
    std::vector<CinDBColData> coldata;

    char str[10000];
    if (input)
    {
        // get column names and create data structure for results 
        input.getline(str, 10000);
        std::vector<std::string> colnames;
        this->split(str, ',', colnames);
        CinDBColData data;
        QString trimmer;
        for (std::vector<std::string>::iterator cur = colnames.begin(); 
             cur != colnames.end(); ++cur)
        {
            trimmer = cur->c_str();
            trimmer = trimmer.trimmed();
            data.type = CinDBColData::UNDEFINED;
            data.min  = 0.0;
            data.max  = 0.0;
            data.name = trimmer.toStdString().c_str();
            coldata.push_back(data);
        }
        // print out col names
        for (std::vector<CinDBColData>::iterator cur = coldata.begin(); 
            cur != coldata.end(); ++cur)
        {
            qWarning() << cur->name.c_str() << "\n";
        }

        // continue, gathering metadata along the way
        std::vector<std::string> colvals;
        while (input.getline(str, 10000))
        {
            qWarning() << str;

            // get values for the current line
            this->split(str, ',', colvals);

            // save relevant metadata
            std::vector<CinDBColData>::iterator curColData = coldata.begin();
            QString value;
            for (std::vector<std::string>::iterator cur = colvals.begin(); 
                 cur != colvals.end(); ++cur)
            {
                // determine type
                qWarning() << "TYPE: " << curColData->type;
                if (curColData->type == CinDBColData::UNDEFINED) {
                    if (isInt(*cur)) {
                        curColData->type = CinDBColData::INT;
                    } else if (isFloat(*cur)) {
                        curColData->type = CinDBColData::FLOAT;
                    } else {
                        curColData->type = CinDBColData::STRING;
                    }
                }

                // save the range
                value = cur->c_str();
                this->adjustRange(*curColData, value.toFloat());

                curColData++;
            }

            colvals.clear();
        }

        // check values
/*
        for (std::vector<CinDBColData>::iterator curColData = coldata.begin();
             curColData != coldata.end(); ++curColData)
        {
            qWarning() << curColData->name.c_str() << "::" 
                << curColData->type 
                << ": " << curColData->min << ", " << curColData->max;
        }
*/
    }

    return res;
}


void CinDBReader::split(const std::string& s, char c, std::vector<std::string>& v) 
{
   int i = 0;
   int j = s.find(c);

   while (j >= 0) {
      v.push_back(s.substr(i, j-i));
      i = ++j;
      j = s.find(c, j);

      if (j < 0) {
         v.push_back(s.substr(i, s.length()));
      }
   }
}

void CinDBReader::adjustRange(CinDBColData &c, float value)
{
    qWarning() << c.name.c_str() << ": " << c.rangeInitialized << ": " << value << ", " << c.min << ", " << c.max;
    if (c.rangeInitialized) {
        if (c.min > value) {
            c.min = value;
        }
        if (c.max < value) {
            c.max = value;
        }
    } else {
        c.rangeInitialized = true;
        c.min = value;
        c.max = value;
    }
    qWarning() << c.name.c_str() << ": " << c.rangeInitialized << ": " << value << ", " << c.min << ", " << c.max << "\n";
}

bool CinDBReader::isInt(const std::string &s) 
{
    bool result = true;

    QString val = s.c_str();
    int startID = 0;
    if (val[0] == '-') {
        // this is a negative value, and so far OK
        startID = 1;
    }

    for (int i=startID;i<val.size();i++) 
    {
        if (! val[i].isDigit())
        {
            result = false;
        }
    }

    return result;
}

bool CinDBReader::isFloat(const std::string &s) 
{
    bool result = true;

    QString val = s.c_str();
    int startID = 0;
    if (val[0] == '-') {
        // this is a negative value, and so far OK
        startID = 1;
    }

    for (int i=startID;i<val.size();i++) 
    {
        if (! val[i].isDigit())
        {
            if (! (val[i] == '.')) {
                result = false;
            }
        }
    }

    return result;
}
