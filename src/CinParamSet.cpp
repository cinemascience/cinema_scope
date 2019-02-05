#include "CinParamSet.h"
#include "CinParameter.h"
#include "CinDatabase.h"
#include <QObject>
#include <QDebug>
#include <QMap>
#include <QString>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStringList>
#include <QVariant>

CinParamSet::CinParamSet() 
{
}

/*! \brief Add uniquely named parameters
 *
 *  Adds uniquely named parameters. The class maintains a list of parameter names in the 
 *  order they were added as well.
 */
bool CinParamSet::add(const QString &name, CinParameter::Type type)
{
    bool retVal = false;

    if (not contains(name)) 
    {
        CinParameter *param = new CinParameter(name, type);

        mParamNames.push_back(name);
        mParameters.insert(name, param); 

        QObject::connect(param, SIGNAL(valueChanged(const QString &, int)), this, SLOT(onParameterValueChanged()));
        retVal = true;
    }

    return retVal;
}

/*
bool CinParamSet::getValue(const QString &name, float &value) 
{
    bool success = false;

    QMap<QString, CinParameter*>::iterator found = mParameters.find(name);
    if (found != mParameters.end())
    {
        value = found.value()->getValue(); 
        success = true;
    }

    return success;
}
*/

/*
bool CinParamSet::getMinMax(const QString &name, float &min, float &max)
{
    bool success = false;

    QMap<QString, CinParameter*>::iterator found = mParameters.find(name);
    if (found != mParameters.end())
    {
        min = found.value()->getMin(); 
        max = found.value()->getMax(); 
        success = true;
    }

    return success;
}
*/

/*
void CinParamSet::changeParameter(const QString &name, float value)
{
    QMap<QString, CinParameter*>::iterator found = mParameters.find(name);
    // qDebug() << "CINPARAMSET: " << name;
    if (found != mParameters.end())
    {
        found.value()->setValue(value);
        emit parameterChanged();
        // qDebug() << "CINPARAMSET emit:" << name << value;
    }
}
*/

bool CinParamSet::contains(const QString &name)
{
    QMap<QString, CinParameter*>::iterator found = mParameters.find(name);

    return (found != mParameters.end()); 
}

void CinParamSet::print()
{
    /*
    QMap<QString, CinParameter*>::iterator cur;
    // qDebug() << "CINPARAMSET: (parameters)";
    for (cur = mParameters.begin(); cur != mParameters.end(); ++cur)
    {
        qDebug() << cur.value()->getName();
        qDebug() << "  " << CinParameter::GetNameForType(cur.value()->getType());
        qDebug() << "  " << cur.value()->getValue();
        qDebug() << "  " << cur.value()->getMin() << ", " << cur.value()->getMax();
    }
    qDebug() << " ";
    */
}


void CinParamSet::init(CinDatabase &db)
{
    // first clear everything
    this->clear();

    // now load new data 
    QString curColumn;
    QSqlQuery query(db.getDatabase());
    QSqlRecord record = db.getDatabase().record(db.getTableName());
    const QStringList &cols = db.getParameterColumnNames();
    int curType;
    for (int i=0;i<cols.count();i++)
    {
        // add the parameter 
        QVariant variant = record.field(cols.at(i)).type();
        if (QString(variant.typeName()) == "int")
        {
            curType = CinParameter::INT;
            // qDebug() << "FIELD TYPE is INT";
        } else if (QString(variant.typeName()) == "double")
        {
            curType = CinParameter::FLOAT;
            // qDebug() << "FIELD TYPE is DOUBLE";
        } else 
        {
            curType = CinParameter::STRING;
            // qDebug() << "FIELD TYPE is STRING";
        }

        add(cols.at(i), CinParameter::FLOAT); 

        // gather all the values
        query.exec("SELECT DISTINCT [" + cols.at(i) + "] FROM " + db.getTableName() + " ORDER BY [" + cols.at(i) + "]"); 
        // TODO make this a reference, instead of a pointer
        CinParameter *param = getParameter(cols.at(i));
        if (param)
        {
            while (query.next())
            {
                if (curType == CinParameter::INT) {
                    // this call is currently not differentiated from FLOAT
                    param->recordValue(query.value(0).toFloat());

                } else if (curType == CinParameter::FLOAT) {
                    param->recordValue(query.value(0).toFloat());

                } else {
                    // not implemented
                    qDebug() << "WARNING: STRING type not handled yet";
                }
            }
            // param->finalizeValues();
            param->setToValueAt(0);
            // param->print();
        } else {
            qWarning() << "PARAMSET: NULL pointer from getParameter";
        }
    }
}

CinParameter *CinParamSet::getParameter(const QString &name)
{
    QMap<QString, CinParameter*>::iterator found = mParameters.find(name);
    if (found != mParameters.end())
    {
        return found.value();
    }

    return NULL;
}



/*
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
*/

void CinParamSet::clear()
{
    mParameters.clear();
    mParamNames.clear();
}

void CinParamSet::onParameterValueChanged()
{
    // qDebug() << "CINPARAMSET onParameterValueChanged";
    emit parameterChanged();
}

void CinParamSet::increment(const QString &name)
{
    CinParameter *param = getParameter(name);

    if (param)
    {
        param->incrementValue();
    }
}

void CinParamSet::decrement(const QString &name)
{
    CinParameter *param = getParameter(name);

    if (param)
    {
        param->decrementValue();
    }
}

bool CinParamSet::parameterExists(const QString &name)
{
    QMap<QString, CinParameter*>::iterator found = mParameters.find(name);
    return (found != mParameters.end());
}
