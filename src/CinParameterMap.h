#ifndef CINPARAMETERMAP_H 
#define CINPARAMETERMAP_H 

#include <QObject>
#include <QString>
#include <QMap>

//! Maps parameters in a database to parameters in a UI 
/*!
 *  Example: if an application expects Phi/Theta, but
 *  the database has no parameters named 'phi' and 
 *  'theta', what parameters should be used instead?
*/
class CinParameterMap : public QObject
{
    Q_OBJECT

    public:
        // constructor
        CinParameterMap();

        bool map(const QString &input, const QString &output);
        bool getInput(const QString &output, QString &input);

    private:
        QMap<QString, QString> mMap;
};

#endif // CINPARAMETERMAP_H
