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

    std::ifstream input(path);

    char str[10000];
    if (input)
    {
        std::string line;
        while (input.getline(str, 10000))
        {
            qWarning() << str;
        }
    }

    return res;
}

