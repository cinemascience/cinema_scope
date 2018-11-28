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
        bool add(const QString &name, CinParameter::Type type, float min, float max, float value);
        // bool remove(conast QString &name);

        // parameter value query API
        bool getNextValue(const QString &key, float value, float &next);        /*!< get the next value; report success/faliure */
        bool getPrevValue(const QString &key, float value, float &prex);        /*!< get the prev value; report success/faliure */
        bool getValue(const QString &name, float &value);   /*!< get the current value of a parameter */
        bool getMinMax(const QString &name, float &min, float &max); 

        // constituent data access
        const QStringList &getParameterNames() const {return mParamNames;}
        int  getNumParameters() {return mParamNames.count();}
        CinParameter *getParameter(const QString &name);

            // TODO make this a const reference
        void init(CinDatabase &db);

        void changeParameter(const QString &name, float value);

        void print();

    signals:
        void parameterChanged(const QString &name, float value);

    private:
        bool contains(const QString &name);

        QStringList                  mParamNames;
        QMap<QString, CinParameter>  mParameters;

    public:
        QMap<QString, CinParameter> &getParameters() {return mParameters;}

};

#endif // CINPARAMSET_H
