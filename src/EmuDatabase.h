#ifndef EMUDATABASE_H 
#define EMUDATABASE_H 

#include <QString>
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

    private:
        QString         mPath;
};

#endif // EMUDATABASE_H
