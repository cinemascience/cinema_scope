#ifndef EMULOADER_H 
#define EMULOADER_H 

#include <QString>
#include "EmuDatabase.h"

//! A class that loads an emu 
/*!
*/
class EmuLoader
{
    public:
        EmuLoader();
        ~EmuLoader();

        bool load(const QString &path);
        void setDatabase(EmuDatabase *db);

    private:
        EmuDatabase    *mDatabase=0;
        QString         mPath;
};

#endif // EMULOADER_H
