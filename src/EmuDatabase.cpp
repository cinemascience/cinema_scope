#include <QFileInfo>
#include <QDebug>
#include "EmuResult.h"
#include "EmuDatabase.h"

int EmuDatabase::NextResultID = -1;

EmuDatabase::EmuDatabase()
{
}

EmuDatabase::~EmuDatabase()
{
}

int EmuDatabase::getNextResultID()
{
    return ++NextResultID;
}

bool EmuDatabase::load(const QString &path)
{
    bool result = false;

    if (QFileInfo::exists(path) && QFileInfo(path).isDir())
    {
        mPath = path;
    }

    return result;
}

int EmuDatabase::addResult(QString name, QString notes, const QVector<double> &inputs, const QVector<double> &results)
{
    EmuResult newResult;
    newResult.setName(name);
    newResult.setNotes(notes);
    newResult.setInputs(inputs);
    newResult.setResults(results);

    int ID = getNextResultID();
    mResults.insert(ID, newResult);

    return ID; 
}

const EmuResult &EmuDatabase::getResult(int ID)
{
    QMap<int, EmuResult>::iterator i = mResults.find(ID);

    return i.value();
}

void EmuDatabase::print()
{
    QMapIterator<int, EmuResult> i(mResults);

    while (i.hasNext()) 
    {
        i.next();
        qDebug() << i.key();
    }
}
