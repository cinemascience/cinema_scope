#include "CinDBView.h"
#include "CinCore.h"
#include "CinDatabase.h"
#include <QtGlobal>
#include <QSqlQuery>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QObject>

CinDBView::CinDBView()
{
    mArtifact = "FILE";
    
    QObject::connect(&mParams, SIGNAL(parameterChanged(const QString &, float)), 
                     this,    SLOT(onParameterChanged(const QString &, float)));
}

void CinDBView::setDatabase(CinDatabase *db)
{
    if (mDatabase == NULL) {
        mDatabase = db;
    } else {
        qWarning("CINDBVIEW: database already exists");
    }
}

int CinDBView::initializeAttributes()
{
    int result = CinCore::ERROR; 

    if (mDatabase != NULL) {
        mParams.init(*mDatabase);
        mArtifacts.init(*mDatabase);

    } else {
        qWarning("CINDBVIEW: NULL database");
    }

    return result;
}

void CinDBView::updateArtifacts()
{
    QString queryString;
    getArtifactQueryString(queryString);

    QSqlQuery query(mDatabase->getDatabase());
    query.exec(queryString);
    // TODO there should only be one; error if this is not the case
    QString artifact;
    while (query.next())
    {
        artifact = query.value(0).toString();
    }

    QString path;
    if (not  getFullPathToArtifact(artifact, path))
    {
        qDebug() << "CINDBVIEW: Artifact check error";
    }
    
    // for now, there is only one artifact that changes, but this should 
    // but updated to a more general emit mechanism
    // qDebug() << "CINDBVIEW emit: " << path << result;
    if (mArtifacts.get(mArtifact) != path)
    {
        mArtifacts.set(mArtifact, path);
        emit artifactChanged(mArtifact, path);
        // qDebug() << "setting artifact    : " << path;
    } else {
        // qDebug() << "artifact already set: " << path;
    }
}

/*! \brief Get a fully qualified and verified path to an artifact
 *
 *  This class knows about the database and the specific artifacts, 
 *  so it can construct a fully qualified path.
 *  Returns a boolean indicating if the file exists
*/
bool CinDBView::getFullPathToArtifact(const QString &artifact, QString &fullpath)
{
    fullpath = QDir(mDatabase->getPath()).filePath(artifact);

    QFileInfo check(fullpath);
    
    return (check.exists() && check.isFile());
}
    

void CinDBView::getArtifactQueryString(QString &query)
{
    query = QString("SELECT [%1] FROM %2 WHERE [").arg(mArtifact, mDatabase->getTableName());

    QMap<QString, CinParameter*>::const_iterator i = mParams.getParameters().constBegin();
    bool first = true;
    while ( i != mParams.getParameters().constEnd() )
    {
        if (not first)
        {
            query += " AND [";
        } else {
            first = false;
        }
        query += QString("%1]=%2").arg(i.key(), QString::number(i.value()->getValue()));
        ++i;
    }

    // qDebug() << "CINDBVIEW: " << query;
}

void CinDBView::onParameterChanged(const QString &key, float value)
{
    updateArtifacts(); 
    // qDebug() << "ONPARAMCHANGED: " << key << ", " << value;
}

void CinDBView::reset()
{
    mDatabase->reset();
    mParams.clear();
    mArtifacts.clear();
}

// TODO parameterSet and ArtifactSet should have the same base class
//      of 'set-ness'
bool CinDBView::parameterExists(const QString &parameter)
{
    float value;
    return mParams.getValue(parameter, value);
}

bool CinDBView::artifactExists(const QString &artifact)
{
    return (mArtifacts.get(artifact) != CinArtifactSet::NOTFOUND);
}


