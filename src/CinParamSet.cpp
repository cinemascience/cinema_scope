#include "CinParamSet.h"
#include "CinParameter.h"
#include "CinDatabase.h"
#include <QDebug>
#include <QMap>
#include <QString>
#include <QSqlQuery>
#include <QStringList>

CinParamSet::CinParamSet() 
{
}

/*! \brief Add uniquely named parameters
 *
 *  Adds uniquely named parameters. The class maintains a list of parameter names in the 
 *  order they were added as well.
 */
bool CinParamSet::add(const QString &name, CinParameter::Type type, float min, float max, float value)
{
    bool retVal = false;

    if (not contains(name)) 
    {
        mParamNames.push_back(name);
        mParameters.insert(name, CinParameter(name, type, min, max, value));
        retVal = true;
    }

    return retVal;
}

bool CinParamSet::getValue(const QString &name, float &value) 
{
    bool success = false;

    QMap<QString, CinParameter>::iterator found = mParameters.find(name);
    if (found != mParameters.end())
    {
        value = found.value().getValue(); 
        success = true;
    }

    return success;
}

bool CinParamSet::getMinMax(const QString &name, float &min, float &max)
{
    bool success = false;

    QMap<QString, CinParameter>::iterator found = mParameters.find(name);
    if (found != mParameters.end())
    {
        min = found.value().getMin(); 
        max = found.value().getMax(); 
        success = true;
    }

    return success;
}

void CinParamSet::changeParameter(const QString &name, float value)
{
    QMap<QString, CinParameter>::iterator found = mParameters.find(name);
    // qDebug() << "CINPARAMSET: " << name;
    if (found != mParameters.end())
    {
        found.value().setValue(value);
        emit parameterChanged(name, value);
        // qDebug() << "CINPARAMSET emit:" << name << value;
    }
}

bool CinParamSet::contains(const QString &name)
{
    QMap<QString, CinParameter>::iterator found = mParameters.find(name);

    return (found != mParameters.end()); 
}

void CinParamSet::print()
{
    QMap<QString, CinParameter>::iterator cur;
    // qDebug() << "CINPARAMSET: (parameters)";
    for (cur = mParameters.begin(); cur != mParameters.end(); ++cur)
    {
        qDebug() << cur.value().getName();
        qDebug() << "  " << CinParameter::GetNameForType(cur.value().getType());
        qDebug() << "  " << cur.value().getValue();
        qDebug() << "  " << cur.value().getMin() << ", " << cur.value().getMax();
    }
    qDebug() << " ";
}


void CinParamSet::init(CinDatabase &db)
{
    // TODO: clear everything out first
    QString curColumn;
    QSqlQuery query;
    const QStringList &cols = db.getParameterColumnNames();
    for (int i=0;i<cols.count();i++)
    {
        // get the min and max values
        query.exec("SELECT MIN(" + cols.at(i) + ") , MAX(" + cols.at(i) + ") FROM " + db.getTableName());
        query.first();

        // add the parameter 
        float min = query.value(0).toFloat(); 
        float max = query.value(1).toFloat(); 
        add(cols.at(i), CinParameter::FLOAT, min, max, min); 

        // gather all the values
        query.exec("SELECT DISTINCT " + cols.at(i) + " FROM " + db.getTableName() + " ORDER BY " + cols.at(i) ); 
        // TODO make this a reference, instead of a pointer
        CinParameter *param = getParameter(cols.at(i));
        if (param)
        {
            while (query.next())
            {
                param->recordValue(query.value(0).toFloat());
                // qDebug() << "PARAMSET: " << query.value(0).toFloat();
            }
            param->sortValues();
            param->print();
        } else {
            qWarning() << "PARAMSET: NULL pointer from getParameter";
        }
    }
}

CinParameter *CinParamSet::getParameter(const QString &name)
{
    QMap<QString, CinParameter>::iterator found = mParameters.find(name);
    if (found != mParameters.end())
    {
        return &(found.value());
    }

    return NULL;
}

bool CinParamSet::getNextValue(const QString &name, float value, float &next)
{
    CinParameter *p = getParameter(name);

    if (p) 
    {
        return p->getNextValue(value, next);
    } else {
        return false;
    }
}

bool CinParamSet::getPrevValue(const QString &name, float value, float &prev)
{
    CinParameter *p = getParameter(name);
    if (p) 
    {
        return p->getPrevValue(value, prev);
    } else 
    {
        return false;
    }
}

bool CinParamSet::valueExists(const QString &key, float value)
{
    CinParameter *p = getParameter(key);
    if (p) 
    {
        return p->valueExists(value);
    } else 
    {
        return false;
    }
}
