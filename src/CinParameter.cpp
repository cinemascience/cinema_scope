#include "CinParameter.h"
#include "CinCore.h"
#include <QString>
#include <QDebug>

const char *CinParameter::TypeNames[] = {"UNDEFINED", "STRING", "FLOAT", "INT"};
const char *CinParameter::NAN_VALUE   = "NaN";
const char *CinParameter::NULL_VALUE  = 0; 
const float CinParameter::NO_VALUE    = -0.0001010101;
const float CinParameter::NO_PREV     = -0.0002020202;
const float CinParameter::NO_NEXT     = -0.0003030303;
const float CinParameter::NOT_SET     = -0.0004040404;

/*! \brief Convenience function to print names instead of ints
 *
 */
const char *CinParameter::GetNameForType(CinParameter::Type type)
{
    return TypeNames[type];
}

CinParameter::CinParameter(const QString &name, CinParameter::Type type)
{
    mName     = name;
    mType     = type;
    mCurID    = 0;
}


bool CinParameter::recordValue(int value)
{
    bool result = false;

    if (indexOf(value) < 0)
    {
        mValues.append(QVariant(value));
        result = true;
    }        

    return result;
}
bool CinParameter::recordValue(double value)
{
    bool result = false;

    if (indexOf(value) < 0)
    {
        mValues.append(QVariant(value));
        result = true;
    }        

    return result;
}
bool CinParameter::recordValue(const QString &value)
{
    bool result = false;

    if (indexOf(value) < 0)
    {
        mValues.append(QVariant(value));
        result = true;
    }        

    return result;
}

void CinParameter::incrementValue()
{
    mCurID++;

    if (mCurID > getLastID())
    {
        // wrap around to start
        mCurID = 0;
    }

    setToValueAt(mCurID);
}

void CinParameter::decrementValue()
{
    mCurID--;

    if (mCurID < 0) 
    {
        // wrap around to end
        mCurID = getLastID();
    }

    setToValueAt(mCurID);
}

void CinParameter::getValueAsString(QString &value)
{
    getValueAsString(value, getCurID());
}

bool CinParameter::getValueAsString(QString &value, int i)
{
    bool result = false;

    if (isValidID(i))
    {
        QVariant variant = mValues.at(i);

        if (getType() == CinParameter::INT)
        {
            value = variant.toString();
        } else if (getType() == CinParameter::FLOAT)
        {
            value = QString::number(variant.toFloat()); 
        } else 
        { 
            value = variant.toString();
        }
        
        result = true;
    }

    return result;
}

bool CinParameter::isValidID(int valueID)
{
    bool result = false;

    if ((valueID >= 0) && (valueID <= getLastID()))
    {
        result = true;
    }

    return result;
}

void CinParameter::print()
{
    qDebug() << "PRINTING";
    for (int i=0;i<mValues.size();i++)
    {
        qDebug() << "    " << i << ": " << mValues.at(i).toString();
    }
}


bool CinParameter::valueExists(int value)
{
    QVariant variant(value);
    return (mValues.indexOf(variant) >= 0);
}

bool CinParameter::valueExists(double value)
{
    QVariant variant(value);
    return (mValues.indexOf(variant) >= 0);
}

bool CinParameter::valueExists(const QString &value)
{
    QVariant variant(value);
    return (mValues.indexOf(variant) >= 0);
}

bool CinParameter::setValue(int value)
{
    bool result = false;

    if (valueExists(value))
    {
        mCurID = indexOf(value);
        result = true;
        postSetValue();
    }

    return result;
}
bool CinParameter::setValue(double value)
{
    bool result = false;

    if (valueExists(value))
    {
        mCurID = indexOf(value);
        result = true;
        postSetValue();
    }

    return result;
}
bool CinParameter::setValue(const QString &value)
{
    bool result = false;

    if (valueExists(value))
    {
        mCurID = indexOf(value);
        result = true;
        postSetValue();
    }

    return result;
}

int CinParameter::indexOf(int value)
{
    QVariant variant(value);
    return mValues.indexOf(variant);
}
int CinParameter::indexOf(double value)
{
    QVariant variant(value);
    return mValues.indexOf(variant);
}
int CinParameter::indexOf(const QString &value)
{
    QVariant variant(value);
    return mValues.indexOf(variant);
}

void CinParameter::postSetValue()
{
    QString value;
    getValueAsString(value);
    emit valueChanged(value, getCurID());
}

bool CinParameter::setToValueAt(int ID)
{
    bool result = false;

    if (isValidID(ID))
    {
        mCurID = ID;
        result = true;
        postSetValue();
    }

    return result;
}
