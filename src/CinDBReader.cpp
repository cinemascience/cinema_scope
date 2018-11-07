#include "CinDBReader.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <fstream>
#include <iostream>
#include <QtGlobal>
#include <QtDebug>
#include <string>
#include <vector>


CinDBReader::CinDBReader() 
{
}

int CinDBReader::readCinemaDatabase(QSqlDatabase &db, const char *path)
{
    int res = 0;

    QString csvFile = path;
    csvFile += "data.csv";
    std::ifstream input(csvFile.toStdString().c_str());
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

    // is this a NaN?
    if (value.toLower() == "nan") {
        type = CinDBColData::FLOAT;
    } else if (value.isEmpty()) {
        type = CinDBColData::UNDEFINED;
    } else {
        value.toInt(&iTest, 10);
        value.toFloat(&fTest);
        if (iTest) {
            type = CinDBColData::INT;
        } else if (fTest) {
            type = CinDBColData::FLOAT;
        } else {
            type = CinDBColData::STRING;
        }
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

int  CinDBReader::loadDB(QSqlDatabase &db, const char *path, std::vector<CinDBColData> &coldata)
{
    QString csvFile = path;
    csvFile += "data.csv";
    std::ifstream input(csvFile.toStdString().c_str());
    QSqlQuery query;
    QString command;
    QString insert;

    // read settings, if they exist
    QString settingsPath = path;
    settingsPath += "cscope.json";
    readSettings(settingsPath.toStdString().c_str());

    // create the table from the columns
    const char *dbname = "cinema_init";
    constructCommands(dbname, coldata, command, insert);
    bool success = query.exec(command); 
    // qDebug() << "Executing creation of \"" << dbname << "\" table" << success; 

    // perform value-based insert
    char str[10000];
    // eat the first line (column names)
    input.getline(str, 10000);
    // get the rest of the values
    if (input)
    {
        std::vector<std::string> colvals;
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
            colvals.clear();
        }
    }

    QString newTableCommand;
    this->constructNewTableCommand(newTableCommand, dbname, "cinema");
    bool newTable = query.exec(newTableCommand); 
    qDebug() << "NEWTABLE : " << newTable;
    bool dropTable = query.exec("DROP TABLE cinema_init");
    qDebug() << "DROPTABLE: " << dropTable; 

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
                 << query.value(6).toString();
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

void CinDBReader::constructNewTableCommand(QString &newTableCommand, const char *initTable, const char *finalTable)
{
    bool first = true;

    newTableCommand = "CREATE TABLE ";
    newTableCommand += finalTable;
    newTableCommand += " AS SELECT ";

    for (std::vector<QString>::iterator cur = mColOrder.begin(); cur != mColOrder.end(); ++cur)
    {
        if (!first) {
            newTableCommand += ", ";
        } else {
            first = false;
        }

        newTableCommand += cur->toStdString().c_str();
    }

    newTableCommand += " FROM ";
    newTableCommand += initTable; 
}



void CinDBReader::readSettings(const char *path)
{
    QString settings;
    QFile file;

    file.setFileName(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    settings = file.readAll();
    file.close();

    QJsonDocument sd = QJsonDocument::fromJson(settings.toUtf8());
    QJsonObject settingsObj = sd.object();
    QJsonObject::const_iterator it = settingsObj.constFind(QString("colorder"));
    QJsonArray value = (*it).toArray();
    for (QJsonArray::iterator cur = value.begin(); cur != value.end(); ++cur)
    {
        mColOrder.push_back((*cur).toString());
    }
    qDebug() << mColOrder;
}
