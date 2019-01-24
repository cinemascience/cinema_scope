#include "CinParameterBase.h"
#include "CinCore.h"
#include <QString>
#include <QDebug>

const char *CinParameterBase::TypeNames[] = {"UNDEFINED", "STRING", "FLOAT", "INT"};
const float CinParameterBase::NO_VALUE = -0.0001010101;
const float CinParameterBase::NO_PREV  = -0.0002020202;
const float CinParameterBase::NO_NEXT  = -0.0003030303;
const float CinParameterBase::NOT_SET  = -0.0004040404;

/*! \brief Convenience function to print names instead of ints
 *
 */
const char *CinParameterBase::GetNameForType(CinParameterBase::Type type)
{
    return TypeNames[type];
}

CinParameterBase::CinParameterBase(const QString &name, CinParameterBase::Type type)
{
    mName     = name;
    mType     = type;
    mCurID    = 0;
}

