#ifndef CINDBINTERFACE_H 
#define CINDBINTERFACE_H 

#include <QObject>
#include "CinArtifactSet.h"
#include "CinParamSet.h"

class CinDatabase;

//! A class that manages a set of artifacts and parameters
/*!
*/
class CinDBInterface : public QObject
{
    Q_OBJECT

    public:
        // constructor
        CinDBInterface() {}

        void setDatabase(CinDatabase *db);
        int load(const QString &db, const QString &table);

    private:
        CinArtifactSet mArtifacts;
        CinParamSet    mParams;
        CinDatabase   *mDatabase;

    public:
        CinArtifactSet *getArtifacts()  {return &mArtifacts;}
        CinParamSet    *getParameters() {return &mParams;}
        CinDatabase    *getDatabase()   {return mDatabase;}

    signals:

};

#endif // CINDBINTERFACE_H
