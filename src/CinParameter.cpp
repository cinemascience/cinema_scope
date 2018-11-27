#include "CinParameter.h"
#include <QString>
#include <QDebug>

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

bool CinParameter::getNextValue(float value, float &next)
{
    float retVal = false; 

    std::vector<float>::iterator it = std::upper_bound(mValues.begin(), mValues.end(), value);

    if (it != mValues.end()) 
    { 
        next = *it;
        retVal = true;
    }        
         
    return retVal; 
}

bool CinParameter::getPrevValue(float value, float &prev)
{
    float retVal = false; 

    std::vector<float>::iterator it = std::lower_bound(mValues.begin(), mValues.end(), value);

    if (it != mValues.end()) 
    { 
        prev = *it;
        retVal = true;
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

void CinParameter::sortValues()
{
    std::sort(mValues.begin(), mValues.end());
}

void CinParameter::print()
{
    qDebug() << "PARAMETER: " << mName;
    for (std::vector<float>::iterator it = mValues.begin(); it != mValues.end(); ++it)
    { 
        qDebug() << "           " << *it;
    }        
    qDebug() << " ";
}
