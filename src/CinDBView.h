#ifndef CINDBVIEW_H 
#define CINDBVIEW_H 

#include <QObject>
#include "CinArtifactSet.h"
#include "CinParamSet.h"

class CinDatabase;

//! Manages a view of a cinema database 
/*!
 *  This class manages an interface to a Cinema Database,
 *  which is a set of parameters and a set of artifacts.
 *  There can be multiple interfaces to the database,
 *  which is why this class contains its own parameters and
 *  artifacts, but only points to the database.
 *
 *  emits a SIGNAL (artifactChanged) when an artifact is
 *  changed.
 *
*/
class CinDBView : public QObject
{
    Q_OBJECT

    public:
        // constructor
        CinDBView();

        void setDatabase(CinDatabase *db);
        void setArtifact(const QString &artifact) {mArtifact = artifact;}
        int load(const QString &db);
        void updateArtifacts();
        void reset();
        bool parameterExists(const QString &parameter);
        bool artifactExists(const QString &artifact);

    signals:
        void artifactChanged(const QString &key, const QString &value);

    private:
        void getArtifactQueryString(QString &query);
        bool getFullPathToArtifact(const QString &artifact, QString &fullpath);

        CinArtifactSet mArtifacts;
        CinParamSet    mParams;
        CinDatabase   *mDatabase=NULL;
        QString        mArtifact;

    public:
        CinArtifactSet *getArtifacts()  {return &mArtifacts;}
        CinParamSet    *getParameters() {return &mParams;}
        CinDatabase    *getDatabase()   {return mDatabase;}

    public slots:
        void onParameterChanged(const QString &key, float value);
};

#endif // CINDBVIEW_H
