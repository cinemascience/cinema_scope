#ifndef EMUDATABASE_H 
#define EMUDATABASE_H 

#include <QMap>
#include <QString>
#include <QVector>
#include "EmuResult.h"
#include "EmuDatabase.h"

//! A class that manages emu data for the application 
/*!
*/
class EmuDatabase
{
    public:
        EmuDatabase();
        ~EmuDatabase();

        bool load(const QString &path);
        int  addResult(QString name, QString notes,
                        const QVector<double> &inputs, 
                        const QVector<double> &results);
        const EmuResult &getResult(int ID);
        void print();

    private:
        int getNextResultID();

        static int              NextResultID;
        QString                 mPath;
        QMap<int, EmuResult>    mResults;
};

#endif // EMUDATABASE_H
