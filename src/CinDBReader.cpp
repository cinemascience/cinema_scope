#include "CinDBReader.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
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
            // data.min  = 0.0;
            // data.max  = 0.0;
            data.name = trimmer.toStdString().c_str();
            coldata.push_back(data);
        }
        // print out col names
        /*
        for (std::vector<CinDBColData>::iterator cur = coldata.begin(); 
            cur != coldata.end(); ++cur)
        {
            qDebug() << cur->name.c_str() << "\n";
        }
        */

        // continue, gathering metadata along the way
        std::vector<std::string> colvals;
        while (input.getline(str, 10000))
        {
            // get values for the current line
            this->split(str, ',', colvals);

            // save relevant metadata
            std::vector<CinDBColData>::iterator curColData = coldata.begin();
            QString value;
            for (std::vector<std::string>::iterator cur = colvals.begin(); 
                 cur != colvals.end(); ++cur)
            {
                // determine type
                if (curColData->type == CinDBColData::UNDEFINED) {
                    QString value = cur->c_str();
                    curColData->type = this->getType(value);
                }

                // save the range
                value = cur->c_str();
                // this->adjustRange(*curColData, value.toFloat());

                curColData++;
            }

            colvals.clear();
        }
    }

    // now load data into the database
    this->loadDB(db, path, coldata); 

    return res;
}

CinDBColData::Type CinDBReader::getType(QString &value)
{
    CinDBColData::Type type = CinDBColData::UNDEFINED;
    bool iTest = false;
    bool fTest = false;

    value.toInt(&iTest, 10);
    value.toFloat(&fTest);
    if (iTest) {
        type = CinDBColData::INT;
    } else if (fTest) {
        type = CinDBColData::FLOAT;
    } else {
        type = CinDBColData::STRING;
    }

    return type;
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

/*
void CinDBReader::adjustRange(CinDBColData &c, float value)
{
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
}
*/

int  CinDBReader::loadDB(QSqlDatabase &db, const char *path, std::vector<CinDBColData> &coldata)
{
    std::ifstream input(path);
    QSqlQuery query;
    QString command;
    QString insert;

    // create the table from the columns
    const char *dbname = "cinema";
    constructCommands(dbname, coldata, command, insert);
    qDebug() << "Executing creation of \"" << dbname << "\" table" << query.exec(command);

    // perform value-based insert
    char str[10000];
    // eat the first line (column names)
    input.getline(str, 10000);
    // get the rest of the values
    if (input)
    {
        std::vector<std::string> colvals;
        int inserts = 0;
        QString trimmer;
        while (input.getline(str, 10000))
        {
            // start the insert command 
            query.prepare(insert);

            // get values for the current line, then iterate over them
            this->split(str, ',', colvals);
            // iterate over the column names as well
            QString bindName;
            std::vector<CinDBColData>::iterator curColData = coldata.begin();
            for (std::vector<std::string>::iterator cur = colvals.begin(); 
                 cur != colvals.end(); ++cur)
            {
                bindName = ":";
                bindName += curColData->name.c_str();
                // bind the value according to type 
                if (curColData->type == CinDBColData::STRING) {
                    trimmer = cur->c_str();
                    trimmer = trimmer.trimmed();
                    query.bindValue( bindName, trimmer ); 
                } else if (curColData->type == CinDBColData::INT) {
                    query.bindValue( bindName, std::stoi(*cur) );
                } else {
                    // it's a float
                    query.bindValue( bindName, std::stof(*cur) );
                }

                // increment iterator 
                curColData++;
            }
            // execute
            query.exec();

            // clean up
            inserts++;
            colvals.clear();
        }
        // qDebug() << "INSERTS: " << inserts;
    }

    //testing
/*
    QSqlRecord record = db.record(dbname);
    for (int i=0;i<record.count();i++)
    {
        QSqlField field = record.field(i);
        qDebug() << field.name() << field.value();
    }
    QString select = "SELECT * from ";
    select += dbname;
    query.exec(select);
    while (query.next())
    {
        qDebug() << "VALUE: " << query.value(0).toString()
                 << query.value(11).toString();
    }
*/
}

void CinDBReader::constructCommands(const char *dbname, std::vector<CinDBColData> &coldata, QString &create, QString &insert)
{
    bool first = true;
    QString names[] = {"UNDEFINED", "varchar(100)", "float", "int"};
    QString values;

    create = "CREATE TABLE ";
    create += dbname;
    create += " (";
    insert = "INSERT INTO ";
    insert += dbname;
    insert += "(";
    values = " values(";

    for (std::vector<CinDBColData>::iterator curColData = coldata.begin();
         curColData != coldata.end(); ++curColData)
    {
        if (!first) {
            create += ", ";
            insert += ", ";
            values += ", ";
        } else {
            first = false;
        }

        create += curColData->name.c_str();
        create += " " + names[curColData->type];

        insert += curColData->name.c_str();
        values += ":";
        values += curColData->name.c_str();

    }
    create += ")";
    insert += ")";
    values += ")";
    insert += values;

    // qDebug() << "CREATE: " << create;
    // qDebug() << "INSERT: " << insert;
}


