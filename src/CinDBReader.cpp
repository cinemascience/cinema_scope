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

using namespace cin;

const QString DBReader::CSVFile      = "data.csv";
const QString DBReader::SettingsFile = "csettings.json";
const QString DBReader::InitTableName = "initial_table";

DBReader::DBReader() 
{
}

int DBReader::readCinemaDatabase(QSqlDatabase &db, const QString &path, const QString &tableName) 
{
    int res = 0;

    QString csvFile = path;
    csvFile += "/" + DBReader::CSVFile; 
    qDebug() << "CSVFILE: " << csvFile;
    std::ifstream input(csvFile.toStdString().c_str());
    std::vector<DBColData> coldata;

    char str[10000];
    if (input)
    {
        // get column names and create data structure for results 
        input.getline(str, 10000);
        std::vector<std::string> colnames;
        this->split(str, ',', colnames);
        DBColData data;
        QString trimmer;
        for (std::vector<std::string>::iterator cur = colnames.begin(); 
             cur != colnames.end(); ++cur)
        {
            trimmer = cur->c_str();
            trimmer = trimmer.trimmed();
            data.type = DBColData::UNDEFINED;
            data.name = trimmer.toStdString().c_str();
            coldata.push_back(data);
        }
        // print out col names
        /*
        for (std::vector<DBColData>::iterator cur = coldata.begin(); 
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
            std::vector<DBColData>::iterator curColData = coldata.begin();
            QString value;
            for (std::vector<std::string>::iterator cur = colvals.begin(); 
                 cur != colvals.end(); ++cur)
            {
                // determine type
                if (curColData->type == DBColData::UNDEFINED) {
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
    this->loadDB(db, path, tableName, coldata); 

    return res;
}

DBColData::Type DBReader::getType(QString &value)
{
    DBColData::Type type = DBColData::UNDEFINED;
    bool iTest = false;
    bool fTest = false;

    // is this a NaN?
    if (value.toLower() == "nan") {
        type = DBColData::FLOAT;
    } else if (value.isEmpty()) {
        type = DBColData::UNDEFINED;
    } else {
        value.toInt(&iTest, 10);
        value.toFloat(&fTest);
        if (iTest) {
            type = DBColData::INT;
        } else if (fTest) {
            type = DBColData::FLOAT;
        } else {
            type = DBColData::STRING;
        }
    } 

    return type;
}


void DBReader::split(const std::string& s, char c, std::vector<std::string>& v) 
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

int  DBReader::loadDB(QSqlDatabase &db, const QString &path, const QString &tableName, std::vector<DBColData> &coldata)
{
    QString csvFile = path;
    csvFile += "/" + DBReader::CSVFile; 
    qDebug() << "CSVFILE: " << csvFile;
    std::ifstream input(csvFile.toStdString().c_str());
    QSqlQuery query;
    QString command;
    QString insert;

    // read settings, if they exist
    QString settingsFile = path;
    settingsFile += "/" + DBReader::SettingsFile;
    // qDebug() << "SETTINGSFILE: " << settingsFile;
    this->readSettings(settingsFile);

    // create the table from the columns
    this->constructCommands(DBReader::InitTableName, coldata, command, insert);
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
            std::vector<DBColData>::iterator curColData = coldata.begin();
            for (std::vector<std::string>::iterator cur = colvals.begin(); 
                 cur != colvals.end(); ++cur)
            {
                bindName = ":";
                bindName += curColData->name.c_str();
                // bind the value according to type 
                if (curColData->type == DBColData::STRING) {
                    trimmer = cur->c_str();
                    trimmer = trimmer.trimmed();
                    query.bindValue( bindName, trimmer ); 
                } else if (curColData->type == DBColData::INT) {
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
    this->constructNewTableCommand(newTableCommand, DBReader::InitTableName, tableName);
    bool newTable = query.exec(newTableCommand); 
    qDebug() << "NEWTABLE : " << newTable;
    bool dropTable = query.exec("DROP TABLE " + DBReader::InitTableName);
    qDebug() << "DROPTABLE: " << dropTable; 
}

void DBReader::constructCommands(const QString &tableName, std::vector<DBColData> &coldata, QString &create, QString &insert)
{
    bool first = true;
    QString names[] = {"UNDEFINED", "varchar(100)", "float", "int"};
    QString values;

    create = "CREATE TABLE ";
    create += tableName;
    create += " (";
    insert = "INSERT INTO ";
    insert += tableName;
    insert += "(";
    values = " values(";

    for (std::vector<DBColData>::iterator curColData = coldata.begin();
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

void DBReader::constructNewTableCommand(QString &newTableCommand, const QString &initTable, const QString &finalTable)
{
    bool first = true;

    newTableCommand = "CREATE TABLE ";
    newTableCommand += finalTable;
    newTableCommand += " AS SELECT ";

    if (mColOrder.size() == 0) {
        newTableCommand += "*";
    } else {
        for (std::vector<QString>::iterator cur = mColOrder.begin(); cur != mColOrder.end(); ++cur)
        {
            if (!first) {
                newTableCommand += ", ";
            } else {
                first = false;
            }

            newTableCommand += cur->toStdString().c_str();
        }
    }

    newTableCommand += " FROM ";
    newTableCommand += initTable; 
}



void DBReader::readSettings(QString &path)
{
    QString settings;
    QFile file(path);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
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
}


