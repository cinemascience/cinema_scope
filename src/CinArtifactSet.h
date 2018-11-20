#ifndef CINARTIFACTSET_H
#define CINARTIFACTSET_H

#include <QObject>
#include <QString>
#include <QMap>

//! A class that manages a set of artifacts
/*!
 *  A set of artifacts defines the output values for a Cinema Database
 *  This class manages communication about these artifacts as they
 *  change in a Qt application.
*/
class CinArtifactSet : public QObject
{
    Q_OBJECT

    public:
        // constructor
        CinArtifactSet() {}

        // member functions
        void set(const QString &key, const QString &value); 
        const QString &get(const QString &key);

        static const QString NOTFOUND;

    signals:
        void artifactChanged(const QString &key, const QString &value);

    private:
        bool contains(const QString &key);

        // member variables
        QMap<QString, QString> mArtifacts;
};

#endif // CINARTIFACTSET_H
