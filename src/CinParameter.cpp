#include "CinParameter.h"
#include <QString>

const char *CinParameter::TypeNames[] = {"UNDEFINED", "STRING", "FLOAT", "INT"};

/*! \brief Convenience function to print names instead of ints
 *
 */
const char *CinParameter::GetNameForType(CinParameter::Type type)
{
    return TypeNames[type];
}

CinParameter::CinParameter(const QString &name, CinParameter::Type type, float min, float max, float cur) 
{
    mName  = name;
    mType  = type;
    mMin   = min;
    mMax   = max;
    mValue = cur;
}
