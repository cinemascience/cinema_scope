#ifndef CINARTIFACTSET_H 
#define CINARTIFACTSET_H 

#include <QString>
#include <QMap>
#include "CinDatabase.h"

//! Manages a set of artifacts
/*!
 *  A set of artifacts defines the output values for a Cinema Database
 *  This class manages communication about these artifacts as they
 *  change in a Qt application.
 *
 *  An artifact is a related set of data in permanent storage. It may be
 *  a series of images, a set of data files - just about anything. This
 *  class manages the 'current value' of an artifact. The meaning of
 *  'current value' is defined by the artifact. For example, for a series
 *  of images, the current value is the current image file. The value of
 *  the current image is the URL to that image.
 *
*/
class CinArtifactSet
{
    public:
        // constructor
        CinArtifactSet();

        void set(const QString &key, const QString &value); 
        const QString &get(const QString &key);

        void init(CinDatabase &db);

        static const QString NOTFOUND;

    private:
        bool contains(const QString &key);

        // member variables
        QMap<QString, QString> mArtifacts;
};

#endif // CINARTIFACTSET_H
