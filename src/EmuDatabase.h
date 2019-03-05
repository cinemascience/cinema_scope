#ifndef EMUDATABASE_H 
#define EMUDATABASE_H 

#include <QMap>
#include <QString>
#include "EmuDatabase.h"
#include "EmuResult.h"

//! A class that manages emu data for the application 
/*!
*/
class EmuDatabase
{
    public:
        EmuDatabase();
        ~EmuDatabase();

        bool load(const QString &path);
        void addResult(int ID, const EmuResult &result);

    private:
        QString                 mPath;
        QMap<int, EmuResult &>  mResults;
};

#endif // EMUDATABASE_H
