#include "CinArtifactSet.h"

#include <QObject>
#include <QString>
#include <QMap>

const QString CinArtifactSet::NOTFOUND = "CinArtifactSetNOTFOUND";


void CinArtifactSet::set(const QString &key, const QString &value) 
{
    mArtifacts.insert(key, value); 

    emit artifactChanged(key, value);
}

bool CinArtifactSet::contains(const QString &key)
{
    return mArtifacts.contains(key);
}

const QString &CinArtifactSet::get(const QString &key)
{
    QMap<QString, QString>::iterator found = mArtifacts.find(key);
    if (found != mArtifacts.end())
    {
        return found.value();
    }

    return CinArtifactSet::NOTFOUND; 
}