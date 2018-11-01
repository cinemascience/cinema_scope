#include "CinDBReader.h"
#include <QFileInfo>

CinDBReader::CinDBReader() 
{
}


int CinDBReader::init()
{
    int res = 0;
    return res;
}

int CinDBReader::readCSV(QString path)
{
    int res = 0;
    QFileInfo check_file(path);

    if (check_file.exists() && check_file.isFile()) 
    {
    }

    return res;
}

