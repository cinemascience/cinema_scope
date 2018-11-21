#include "CinDBView.h"
#include "CinCore.h"
#include <QtGlobal>

void CinDBView::setDatabase(CinDatabase *db)
{
    if (mDatabase == NULL) {
        mDatabase = db;
        mParams.setDatabase(db);
        mParams.setArtifacts(&mArtifacts);
        mArtifacts.setDatabase(db);
        mArtifacts.setParameters(&mParams);
    } else {
        qWarning("CINDBVIEW: database already exists");
    }
}

int CinDBView::load(const QString &db, const QString &table)
{
    int result = CinCore::ERROR; 

    if (mDatabase != NULL) {
        result = mDatabase->loadDatabase(db, table);
            // TODO check result
        mParams.init();
        mArtifacts.init();

    } else {
        qWarning("CINDBVIEW: NULL database");
    }

    return result;
}
