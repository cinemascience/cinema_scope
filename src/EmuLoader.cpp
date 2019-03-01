#include "EmuLoader.h"

EmuLoader::EmuLoader()
{
}

EmuLoader::~EmuLoader()
{
}

void EmuLoader::setDatabase(EmuDatabase *db)
{
    if (db && not(mDatabase))
    {
        mDatabase = db;
    } else {
        // TODO: report
    }
}

bool EmuLoader::load(const QString &path)
{
    bool result = false;

    // TODO: check mDatabase and other internals

    result = mDatabase->load(path);

    return result;
}
