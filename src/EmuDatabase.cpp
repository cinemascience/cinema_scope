#include <QFileInfo>
#include "EmuDatabase.h"

EmuDatabase::EmuDatabase()
{
}

EmuDatabase::~EmuDatabase()
{
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
