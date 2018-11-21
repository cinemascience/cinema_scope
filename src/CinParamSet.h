#ifndef CINPARAMSET_H
#define CINPARAMSET_H

#include <QObject>
#include <QString>
#include <QMap>
#include "CinDatabase.h"
#include "CinParameter.h"

class CinArtifactSet;

//! A class that manages a set of parameters 
/*!
 *  A set of parameters defines the input values for a Cinema Database
 *  This class manages communication about these parameters as they
 *  change in a Qt application.
*/
class CinParamSet : public QObject
{
    Q_OBJECT 

    public:
        CinParamSet();

        void setDatabase(CinDatabase *database);
        bool add(const QString &name, CinParameter::Type type, float min, float max, float value);
        void setArtifacts(CinArtifactSet *artifacts);

        bool getValue(const QString &name, float *value); 
        bool getMinMax(const QString &name, float *min, float *max); 
        const QStringList &getParameterNames() const {return mParamNames;}
        int  getNumParameters() {return mParamNames.count();}
        CinParameter *getParameter(const QString &name);

        void initParameters();

        void changeParameter(const QString &name, float value);

        void print();

    signals:
        void parameterChanged(const QString &name, float value);

    private:
        bool contains(const QString &name);

        QStringList                  mParamNames;
        QMap<QString, CinParameter>  mParameters;
        CinDatabase                 *mDatabase;
        CinArtifactSet              *mArtifacts=NULL;

};

#endif // CINPARAMSET_H
