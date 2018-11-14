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

float CinParameter::getClosestValue(float value)
{
    float retVal = value;

    std::vector<float>::iterator it = std::find(mValues.begin(), mValues.end(), value);

    if (it != mValues.end()) 
    { 
        retVal = *it;
    }        
         
    return retVal; 
}

/*! \brief Add unique values to the datastructure
 *
 */
void CinParameter::recordValue(float value)
{
    if (not (std::find(mValues.begin(), mValues.end(), value) != mValues.end()))
    {
        mValues.push_back(value);
    }
}
