#include "CinParameter.h"
#include <QString>
#include <QDebug>

const char *CinParameter::TypeNames[] = {"UNDEFINED", "STRING", "FLOAT", "INT"};
const float CinParameter::NO_VALUE = -0.0001010101;
const float CinParameter::NO_PREV  = -0.0002020202;
const float CinParameter::NO_NEXT  = -0.0003030303;
const float CinParameter::NOT_SET  = -0.0004040404;

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
    mCurValue = cur;
}

/*! \brief Returns next biggest value of parameter
 *
 *  Sets value for 'next' if successful
 *
 *  False if biggest value in array or bigger than largest value
 *  True otherwise
 */
bool CinParameter::getNextValue(float value, float &next)
{
    float result = false; 
    next = CinParameter::NO_NEXT;

    std::vector<float>::iterator it = std::upper_bound(mValues.begin(), mValues.end(), value);

    if (it != mValues.end()) 
    { 
        next   = *it;
        result = true;
    }        

    return result; 
}

bool CinParameter::getPrevValue(float value, float &prev)
{
    float result = false; 
    prev = CinParameter::NO_PREV;

    if (value <= mValues.front())
    {
        // it's the greatest value or greater, so
        // there is no prev value
        result = false;

    } else if (value > mValues.back())
    {
        // it's the greatest value or greater
        prev = mValues.back();
        result = true;

    } else {
        std::vector<float>::iterator it = std::lower_bound(mValues.begin(), mValues.end(), value);
        if (it != mValues.begin()) 
        { 
            --it;
            prev = *it;
            result = true;
        } else {
            // how do we reach this?
        }
    }

    return result; 
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
    if (mValues.size() != 0) 
    {
        std::sort(mValues.begin(), mValues.end());
        // make sure min and max are still correct
        setMin(mValues.front());
        setMax(mValues.back());
    }
}

void CinParameter::print()
{
    qDebug() << "PARAMETER: " << mName << mValues;
}


bool CinParameter::valueExists(float value)
{
    return (std::find(mValues.begin(), mValues.end(), value) != mValues.end());
}


int  CinParameter::getNumValues() 
{ 
    return mValues.size();
}

bool CinParameter::valueAsString(QString &value, int id)
{
    bool result = false;
    float fValue = 0.0;

    result = valueAt(fValue, id);
    value = "";
    if (result)
    {
        value = QString::number(fValue);
    }

    return result;
}

bool CinParameter::setToValueAt(int valueID)
{
    bool result = false;
    float fValue;

    result = valueAt(fValue, valueID);
    if (result)
    {
        setValue(fValue);

        emit valueChanged(getValue(), valueID);
        emit valueChanged(getName(), getValue()); 
    } else {
        // TODO report error
    }

    return result;
}

bool CinParameter::isValidID(int valueID)
{
    return ((valueID >= 0) && (static_cast<unsigned int>(valueID) < mValues.size()));
}

bool CinParameter::valueAt(float &value, int valueID)
{
    bool result = false;

    value = CinParameter::NO_VALUE;
    result = isValidID(valueID);
    if (result)
    {
        value = mValues.at(valueID);
    } else {
        qDebug() << "ERROR: invalid ID passed to CinParameter: " << valueID;
    }

    return result;
}
