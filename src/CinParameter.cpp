#include "CinParameter.h"
#include <QString>
#include <QDebug>

const char *CinParameter::TypeNames[] = {"UNDEFINED", "STRING", "FLOAT", "INT"};
const float CinParameter::NO_PREV = -0.0001010101;
const float CinParameter::NO_NEXT = -0.0002020202;
const float CinParameter::NOT_SET = -0.0003030303;

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
    prev = CinParameter::NO_PREV;


    if (value <= mValues.front())
    {
        // it's the greatest value or greater, so
        // there is no prev value
        retVal = false;

    } else if (value > mValues.back())
    {
        // it's the greatest value or greater
        prev = mValues.back();
        retVal = true;

    } else {
        std::vector<float>::iterator it = std::lower_bound(mValues.begin(), mValues.end(), value);
        if (it != mValues.begin()) 
        { 
            --it;
            prev = *it;
            retVal = true;
        } else {
            // how do we reach this?
        }
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
    // make sure min and max are still correct
    setMin(mValues.front());
    setMax(mValues.back());
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


/*! \brief Run a test of this class 
 *
 */
bool CinParameter::TestClass()
{
    bool retVal = false;

    CinParameter param("test", CinParameter::FLOAT);
    param.recordValue(1.0);
    param.recordValue(4.0);
    param.recordValue(2.0);
    param.recordValue(3.0);
    param.sortValues();

    std::vector<float> data = {0.0, 1.0, 1.5, 2.0, 3.5, 4.0, 4.5};

    qDebug() << "TESTING Class CinParameter (begin)";
    param.print();
    qDebug() << "data: " << data;
    qDebug() << "output is \"TEST TYPE (return value) : value : result\"";

    // Prev
    bool result = false;
    float fResult = 0.0;
    for (std::vector<float>::iterator it = data.begin(); it != data.end(); it++)
    {
        result = param.getPrevValue(*it, fResult);
        qDebug() << "PREV TEST (" << result << ") : " << *it << ": " << fResult;
    }

    // Next
    for (std::vector<float>::iterator it = data.begin(); it != data.end(); it++)
    {
        result = param.getNextValue(*it, fResult);
        qDebug() << "NEXT TEST (" << result << ") : " << *it << ": " << fResult;
    }
    qDebug() << "TESTING Class CinParameter (complete)";
    qDebug() << "";

    return retVal;
}
