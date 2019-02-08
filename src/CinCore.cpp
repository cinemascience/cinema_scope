#include "CinCore.h"
#include <QString>

const int     CinCore::ERROR     = -89897676; 
const int     CinCore::NOTFOUND  = -12397676; 
const QString CinCore::NOVALUE   = "NOVALUE";
const QString CinCore::NAN_VALUE = "NaN";

bool CinCore::IsNAN(const QString &value)
{
    bool result = false;

    if (value.toLower() == "nan")
    {
        result = true;
    }

    return result;
}



