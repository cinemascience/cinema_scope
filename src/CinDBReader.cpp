#include "CinDBReader.h"
#include <fstream>
#include <iostream>
#include <QtGlobal>
#include <QtDebug>

CinDBReader::CinDBReader() 
{
}


int CinDBReader::init()
{
    int res = 0;
    return res;
}

int CinDBReader::readCSV(const char *path)
{
    int res = 0;

    std::fstream input(path, std::fstream::in); 

    qWarning() << "here 1";
    if (input)
    {
        // qDebug() << "here 2" << std::endl;
        for (std::string line; getline( input, line ); )
        {
            // qDebug() << "here 3" << std::endl;
            // qDebug() << line << std::endl;
        }
    }

    return res;
}

