#include "CinParameterVariant.h"
#include "CinCore.h"
#include <QString>
#include <QDebug>

const char *CinParameterVariant::TypeNames[] = {"UNDEFINED", "STRING", "FLOAT", "INT"};
const char *CinParameterVariant::NAN_VALUE = "NaN";
const char *CinParameterVariant::NULL_VALUE = 0; 
const float CinParameterVariant::NO_VALUE = -0.0001010101;
const float CinParameterVariant::NO_PREV  = -0.0002020202;
const float CinParameterVariant::NO_NEXT  = -0.0003030303;
const float CinParameterVariant::NOT_SET  = -0.0004040404;

/*! \brief Convenience function to print names instead of ints
 *
 */
const char *CinParameterVariant::GetNameForType(CinParameterVariant::Type type)
{
    return TypeNames[type];
}

CinParameterVariant::CinParameterVariant(const QString &name, CinParameterVariant::Type type)
{
    mName     = name;
    mType     = type;
    mCurID    = 0;
}


bool CinParameterVariant::recordValue(int value)
{
    bool result = false;

    if (indexOf(value) < 0)
    {
        mValues.append(QVariant(value));
        result = true;
    }        

    return result;
}
bool CinParameterVariant::recordValue(double value)
{
    bool result = false;

    if (indexOf(value) < 0)
    {
        mValues.append(QVariant(value));
        result = true;
    }        

    return result;
}
bool CinParameterVariant::recordValue(const QString &value)
{
    bool result = false;

    if (indexOf(value) < 0)
    {
        mValues.append(QVariant(value));
        result = true;
    }        

    return result;
}

void CinParameterVariant::incrementValue()
{
    mCurID++;

    if (mCurID > getLastID())
    {
        // wrap around to start
        mCurID = 0;
    }
}

void CinParameterVariant::decrementValue()
{
    mCurID--;

    if (mCurID < 0) 
    {
        // wrap around to end
        mCurID = getLastID();
    }
}

void CinParameterVariant::getValueAsString(QString &value)
{
    getValueAsString(value, getCurID());
}

bool CinParameterVariant::getValueAsString(QString &value, int i)
{
    bool result = false;

    if (isValidID(i))
    {
        QVariant variant = mValues.at(i);
        value = variant.toString();
    }

    return result;
}

bool CinParameterVariant::isValidID(int valueID)
{
    bool result = false;

    if ((valueID >= 0) && (valueID <= getLastID()))
    {
        result = true;
    }

    return result;
}

void CinParameterVariant::print()
{
    qDebug() << "PRINTING";
    for (int i=0;i<mValues.size();i++)
    {
        qDebug() << "    " << i << ": " << mValues.at(i).toString();
    }
}


bool CinParameterVariant::valueExists(int value)
{
    QVariant variant(value);
    return (mValues.indexOf(variant) >= 0);
}

bool CinParameterVariant::valueExists(double value)
{
    QVariant variant(value);
    return (mValues.indexOf(variant) >= 0);
}

bool CinParameterVariant::valueExists(const QString &value)
{
    QVariant variant(value);
    return (mValues.indexOf(variant) >= 0);
}

bool CinParameterVariant::setValue(int value)
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
bool CinParameterVariant::setValue(double value)
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
bool CinParameterVariant::setValue(const QString &value)
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

int CinParameterVariant::indexOf(int value)
{
    QVariant variant(value);
    return mValues.indexOf(variant);
}
int CinParameterVariant::indexOf(double value)
{
    QVariant variant(value);
    return mValues.indexOf(variant);
}
int CinParameterVariant::indexOf(const QString &value)
{
    QVariant variant(value);
    return mValues.indexOf(variant);
}

void CinParameterVariant::postSetValue()
{
    QString value;
    getValueAsString(value);
    emit valueChanged(value, getCurID());
}
