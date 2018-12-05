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
#include <QDir>
#include <string>
#include <vector>

const QString CinDBReader::CSVFile       = "data.csv";
const QString CinDBReader::SettingsFile  = "csettings.json";
const QString CinDBReader::InitTableName = "initial_table";
const QString CinDBReader::NOFILE        = "CDBReaderNOFILE";

// TODO remove std::strings and std::vectors
CinDBReader::CinDBReader() 
{
    mCurDatabase     = CinDBReader::NOFILE;
    mCurCSVFile      = CinDBReader::NOFILE;
    mCurSettingsFile = CinDBReader::NOFILE;
}

void CinDBReader::setCurDatabase(const QString &path)
{
    mCurDatabase = path;
    mCurCSVFile      = QDir(path).filePath(CinDBReader::CSVFile);
    mCurSettingsFile = QDir(path).filePath(CinDBReader::SettingsFile);
}

int CinDBReader::VerifyDatabase(const QString &path)
{
    int res = 0;

    return res;
}

int CinDBReader::load(QSqlDatabase &db, const QString &path, const QString &tableName)
{
    int res = CinDBReader::DatabaseLoadError; 

    setCurDatabase(path);
    std::ifstream input(getCSVFile().toStdString().c_str());
    std::vector<CinDBColData> coldata;

    // TODO read files in a bulletproof way. 
    char str[10000];
    if (input)
    {
        // get column names and create data structure for results 
        input.getline(str, 10000);
        std::vector<std::string> colnames;
        split(str, ',', colnames);
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

        // continue, gathering metadata along the way
        std::vector<std::string> colvals;
        while (input.getline(str, 10000))
        {
            // get values for the current line
            split(str, ',', colvals);

            // save relevant metadata
            std::vector<CinDBColData>::iterator curColData = coldata.begin();
            for (std::vector<std::string>::iterator cur = colvals.begin(); 
                 cur != colvals.end(); ++cur)
            {
                // determine type
                if (curColData->type == CinDBColData::UNDEFINED) {
                    QString value = cur->c_str();
                    curColData->type = getType(value);
                }

                curColData++;
            }

            colvals.clear();
        }

        res = CinDBReader::DatabaseLoaded;
        mCurDatabase = path;
    } else {
        res = CinDBReader::DatabaseLoadError;
    }

    // now load data into the database
    loadDB(db, tableName, coldata); 

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

void  CinDBReader::loadDB(QSqlDatabase &db, const QString &tableName, std::vector<CinDBColData> &coldata)
{
    std::ifstream input(getCSVFile().toStdString().c_str());
    QSqlQuery query;
    QString command;
    QString insert;

    // read settings, if they exist
    readSettings();

    // create the table from the columns
    constructCommands(CinDBReader::InitTableName, coldata, command, insert);
    if (query.exec(command) )
    {
        qDebug() << "CINDBREADER: ERROR EXECUTING query";
    }

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
            split(str, ',', colvals);
            // iterate over the column names as well
            QString bindName;
            std::vector<CinDBColData>::iterator curColData = coldata.begin();
            for (std::vector<std::string>::iterator cur = colvals.begin(); 
                 cur != colvals.end(); ++cur)
            {
                bindName = QString(":%1").arg(curColData->name.c_str());
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
    constructNewTableCommand(newTableCommand, CinDBReader::InitTableName, tableName);
    if (not query.exec(newTableCommand)) {
        qDebug() << "NEWTABLE : error in CinDBReader";
    }
    if (not query.exec("DROP TABLE " + CinDBReader::InitTableName))
    {
        qDebug() << "DROPTABLE: error in CinDBReader";
    }
}

void CinDBReader::constructCommands(const QString &tableName, std::vector<CinDBColData> &coldata, QString &create, QString &insert)
{
    bool first = true;
    QString names[] = {"UNDEFINED", "varchar(100)", "float", "int"};
    QString values;

    create = QString("CREATE TABLE %1 (").arg(tableName);
    insert = QString("INSERT INTO %1 (").arg(tableName);
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

        create += QString("%1 %2").arg(curColData->name.c_str(), names[curColData->type]);

        insert += curColData->name.c_str();
        values += QString(":%1").arg(curColData->name.c_str());

    }
    create += ")";
    insert += ")";
    values += ")";
    insert += values;

    // qDebug() << "CREATE: " << create;
    // qDebug() << "INSERT: " << insert;
}

void CinDBReader::constructNewTableCommand(QString &newTableCommand, const QString &initTable, const QString &finalTable)
{
    bool first = true;

    newTableCommand = QString("CREATE TABLE %1 AS SELECT ").arg(finalTable);

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

    newTableCommand += QString(" FROM %1").arg(initTable);

    // qDebug() << "NEW: " << newTableCommand;
}



void CinDBReader::readSettings()
{
    QString settings;
    QFile file(getSettingsFile());

    mColOrder.clear();
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
        // qDebug() << mColOrder;
    }
}


