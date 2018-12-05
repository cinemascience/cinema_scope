#include "CinParameterMap.h"

CinParameterMap::CinParameterMap()
{
}

bool CinParameterMap::map(const QString &input, const QString &output)
{
    bool result = true;

    mMap.insert(output, input); 

    return result;
}

bool CinParameterMap::getInput(const QString &output, QString &input)
{
    bool result = false;

    QMap<QString, QString>::iterator found = mMap.find(output);
    if (found != mMap.end())
    {
        input = found.value();
        result = true;
    }

    return result; 
}

