#ifndef CINCORE_H 
#define CINCORE_H 

#include <QString>

//! Manages Cinema-level data and methods 
/*!
*/
class CinCore
{
    public:
        static const int     ERROR;
        static const int     NOTFOUND;
        static const QString NOVALUE;
        static const QString NAN_VALUE;

        static bool IsNAN(const QString &value);
};

#endif // CINCORE_H
