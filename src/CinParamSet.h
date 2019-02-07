#ifndef CINPARAMSET_H
#define CINPARAMSET_H

#include <QObject>
#include <QString>
#include <QMap>
#include "CinParameter.h"

class CinDatabase;

//! Manages a set of parameters 
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

        // add/remove
        bool add(const QString &name, CinParameter::Type type);
        // TODO: implement 'remove' method
        // bool remove(const QString &name);
        void clear();

        bool parameterExists(const QString &key);
        void increment(const QString &name);
        void decrement(const QString &name);

        // constituent data access
        const QStringList &getParameterNames() const {return mParamNames;}
        int  getNumParameters() {return mParamNames.count();}
        CinParameter *getParameter(const QString &name);

        void init(const CinDatabase &db);

        void print();

    public slots:
        void onParameterValueChanged();

    signals:
        void parameterChanged();

    private:
        bool contains(const QString &name);

        QStringList                  mParamNames;
        QMap<QString, CinParameter*> mParameters;

    public:
        QMap<QString, CinParameter*> &getParameters() {return mParameters;}

};

#endif // CINPARAMSET_H
