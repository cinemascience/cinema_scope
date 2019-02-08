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

        connect(param, &CinParameter::valueChanged, this, &CinParamSet::onParameterValueChanged);
        retVal = true;
    }

    return retVal;
}

bool CinParamSet::contains(const QString &name)
{
    QMap<QString, CinParameter*>::iterator found = mParameters.find(name);

    return (found != mParameters.end()); 
}

void CinParamSet::print()
{
    QMap<QString, CinParameter*>::iterator cur;
    for (cur = mParameters.begin(); cur != mParameters.end(); ++cur)
    {
        QString value;
        cur.value()->getValueAsString(value);
        qDebug() << cur.value()->getName();
        qDebug() << "  " << CinParameter::GetNameForType(cur.value()->getType());
        qDebug() << "  " << value; 
    }
    qDebug() << " ";
}


void CinParamSet::init(const CinDatabase &db)
{
    // first clear everything
    this->clear();

    // now load new data 
    QString curColumn;
    QSqlQuery query(db.getDatabase());
    QSqlRecord record = db.getDatabase().record(db.getTableName());
    const QStringList &cols = db.getParameterColumnNames();
    CinParameter::Type curType;
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

        add(cols.at(i), curType); 

        // gather all the values
        query.exec("SELECT DISTINCT [" + cols.at(i) + "] FROM " + db.getTableName() + " ORDER BY [" + cols.at(i) + "]"); 
        // TODO make this a reference, instead of a pointer
        CinParameter *param = getParameter(cols.at(i));
        if (param)
        {
            while (query.next())
            {
                if (query.value(0) == "")
                {
                    param->recordValue("");
                } else if (query.value(0).toString().toLower() == "\"\"")
                {
                    param->recordValue("\"\"");
                } else if (query.value(0).toString().toLower() == "nan")
                {
                    param->recordValue("NaN");
                } else {
                    if (curType == CinParameter::INT) {
                        // this call is currently not differentiated from FLOAT
                        param->recordValue(query.value(0).toInt());

                    } else if (curType == CinParameter::FLOAT) {
                        param->recordValue(query.value(0).toFloat());

                    } else {
                        param->recordValue(query.value(0).toString());
                    }
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

void CinParamSet::clear()
{
    mParameters.clear();
    mParamNames.clear();
}

void CinParamSet::onParameterValueChanged()
{
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
