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

int CinDBView::load(const QString &db, const QString &table)
{
    int result = CinCore::ERROR; 

    if (mDatabase != NULL) {
        result = mDatabase->loadDatabase(db, table);
            // TODO check result of loading database
        mParams.init(*mDatabase);

    } else {
        qWarning("CINDBVIEW: NULL database");
    }

    return result;
}

void CinDBView::updateArtifacts()
{
    QString query;
    getArtifactQueryString(query);

    QSqlQuery q;
    q.prepare(query); 
    q.exec();
    // TODO there should only be one; error if this is not the case
    QString artifact;
    while (q.next())
    {
        // qDebug() << "VIEW: " << q.value(0);
        artifact = q.value(0).toString();
    }

    QString path;
    bool result = getFullPathToArtifact(artifact, path);
    
    // for now, there is only one artifact that changes, but this should 
    // but updated to a more general emit mechanism
    // qDebug() << "CINDBVIEW emit: " << path << result;
    emit artifactChanged("FILE", path);
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
    // TODO: abstract out FILE here
    query = QString("SELECT %1 FROM %2 WHERE ").arg("FILE", mDatabase->getTableName());

    QMap<QString, CinParameter>::const_iterator i = mParams.getParameters().constBegin();
    bool first = true;
    while ( i != mParams.getParameters().constEnd() )
    {
        if (not first)
        {
            query += " AND ";
        } else {
            first = false;
        }
        query += QString("%1=%2").arg(i.key(), QString::number(i.value().getValue()));
        ++i;
    }

    // qDebug() << "CINDBVIEW: " << query;
}

void CinDBView::onParameterChanged(const QString &key, float value)
{
    updateArtifacts(); 
    // qDebug() << "ONPARAMCHANGED: " << key << ", " << value;
}
