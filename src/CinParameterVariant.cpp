#include "CinParameterVariant.h"
#include "CinCore.h"
#include <QString>
#include <QDebug>

const char *CinParameterVariant::TypeNames[] = {"UNDEFINED", "STRING", "FLOAT", "INT"};
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

bool CinParameterVariant::recordValue(float value)
{
    QVariant variant(value);
    bool result = false;

    if (mValues.indexOf(variant) < 0)
    {
        mValues.append(QVariant(value));
        result = true;
    }        

    return result;
}

bool CinParameterVariant::recordValue(int value)
{
    QVariant variant(value);
    bool result = false;

    if (mValues.indexOf(variant) < 0)
    {
        mValues.append(QVariant(value));
        result = true;
    }        

    return result;
}

bool CinParameterVariant::recordValue(const QString &value)
{
    QVariant variant(value);
    bool result = false;

    if (mValues.indexOf(variant) < 0)
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

bool CinParameterVariant::getValueAsString(QString &value, int i)
{
    bool result = false;

    if (isValidID(i))
    {
        QVariant variant = mValues.at(i);
        value = variant.toString();
    } else {
        value = "";
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
        qDebug() << "    " << mValues.at(i).toString();
    }
}

bool CinParameterVariant::valueExists(float value)
{
    QVariant variant(value);
    return (mValues.indexOf(variant) >= 0);
}

bool CinParameterVariant::valueExists(const QString &value)
{
    QVariant variant(value);
    return (mValues.indexOf(variant) >= 0);
}

bool CinParameterVariant::valueExists(int value)
{
    QVariant variant(value);
    return (mValues.indexOf(variant) >= 0);
}
