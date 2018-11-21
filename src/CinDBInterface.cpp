#include "CinDBInterface.h"
#include <QtGlobal>

void CinDBInterface::setDatabase(CinDatabase *db)
{
    // TODO error if mDatabase != NULL
    mDatabase = db;
    mParams.setDatabase(db);
    mArtifacts.setDatabase(db);
}

int CinDBInterface::load(const QString &db, const QString &table)
{
    int result = -1;

    if (mDatabase) {
        result = mDatabase->loadDatabase(db, table);
        // TODO check result
        mParams.initParameters();
        mParams.print();
        // TODO create init for this class; rename initParameters to init
        // mArtifacts.init();

    } else {
        qWarning("CINDBINTERFACE: NULL database");
    }

    // TODO create a common Cinema set of returns (errors, etc.)
    return result;
}
