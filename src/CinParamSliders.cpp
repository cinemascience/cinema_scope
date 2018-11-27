#include "CinParamSliders.h"
#include <QDebug>
#include <QFileInfo>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QLabel>
#include <QSlider>
#include <QVector>

CinParamSliders::CinParamSliders() 
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mSliderLayout = new QFormLayout(this);    
    mSliderLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    setLayout(mSliderLayout);
}

void CinParamSliders::connect(CinDatabase *cdb, CinParamSet *params)
{
    if (cdb && params) 
    {
        mCurDatabase = cdb;
        mParameters  = params;
        buildSliders();
        QObject::connect(params, SIGNAL(parameterChanged(const QString &, float)), 
                this, SLOT(onParameterValueChanged(const QString &, float)));
        
    } else {
        qWarning() << "ERROR: NULL database passed to CinParamSliders";
    }
}

/*! \brief Build the set of sliders
 *
 */
void CinParamSliders::buildSliders()
{
    QSlider *slider = NULL;
    QString curColumn;
    const QStringList &cols = mParameters->getParameterNames();
    float min, max;
    for (int i=0;i<cols.count();i++)
    {
        if ( mParameters->getMinMax( cols.at(i), min, max ) )
        {
            // create slider
            slider = new QSlider(Qt::Horizontal);
            slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            slider->setRange(min, max);
            QObject::connect(slider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged(int)));
        
            // add these to the layout
            mSliderLayout->addRow(cols.at(i), slider);

        } else {
            qWarning() << "ERROR: failed to get min/max values from parameter set"; 
        }

    }

    constructQueryString();
}

void CinParamSliders::reset()
{
    // Remove all the rows. This deletes the children of the rows as well
    int count = mSliderLayout->rowCount();
    // qDebug() << "SLIDER COUNT: " << count;
    for (int i=(count-1);i>=0;i--)
    {
        mSliderLayout->removeRow(i);
    }

    // clear the query
    mSliderQuery.clear();
}

QSlider *CinParamSliders::getSliderAt(int i)
{
    return dynamic_cast<QSlider *>(mSliderLayout->itemAt(i, QFormLayout::FieldRole)->widget());
}

QLabel *CinParamSliders::getLabelAt(int i)
{
    return dynamic_cast<QLabel *>(mSliderLayout->itemAt(i, QFormLayout::LabelRole)->widget());
}


/*! \brief Find the artifact defined by the current parameters 
 * 
 */
void CinParamSliders::onSliderValueChanged(int value)
{
    QSqlQuery query;

    query.prepare(QString::fromStdString(mSliderQuery.toStdString()));
    int numSliders = mParameters->getNumParameters();
    QString s;
    QSlider *slider = NULL;
    QSlider *label = NULL;
    for(int i=0;i<numSliders;i++)
    {
        s = ":" + getLabelAt(i)->text();
        query.bindValue(s, getSliderAt(i)->value());
    }
    query.exec();

    if (query.first())
    {
        //get the last column which has the image name
        QString imagePath = mCurDatabase->getPath();
        imagePath += "/";
        imagePath += query.value(numSliders).toString(); 

        // if a file exists at this path, return it, otherwise, 
        // return the 'empty' image
        if ( ! (QFileInfo::exists(imagePath) && QFileInfo(imagePath).isFile()) )
        {
            imagePath = mCurDatabase->getPath();
            imagePath += "/";
            imagePath += "empty_image/empty.png";
        }

        emit artifactSelected(imagePath);
        // qDebug() << "DBSLIDER EMIT: " << imagePath;
    }

    popSlidersToValidValue();
}


/*! \brief Manage sliders so they pop to valid values in UI
 * 
 * Pop sliders to a valid value during drag: To overcome Qt's default behavior of sliders 
 * that increment step by 1. But our step is not always 1.
 */
void CinParamSliders::popSlidersToValidValue()
{
    // QString minText, maxText;
    // QSqlQuery minQuery, maxQuery;
    float prevVal, nextVal;

    const QStringList &cols = mParameters->getParameterNames();
    QSlider *slider = NULL;
    QLabel *label = NULL;
    QString name;

    for (int i=0;i<cols.count();i++)
    {
        // TODO: go over logic with SD
        slider = getSliderAt(i);
        name   = cols.at(i);
        float curVal = slider->value();
        mParameters->getNextValue(name, curVal, nextVal);
        mParameters->getPrevValue(name, curVal, prevVal);

        // if ( qAbs(prevVal - static_cast<float>(curVal)) >= qAbs(nextVal - static_cast<float>(curVal)) )
        if ( qAbs(prevVal - curVal) >= qAbs(nextVal - curVal) )
        {
            slider->setValue(nextVal);
            mParameters->changeParameter(name, nextVal);

        } else {
            slider->setValue(prevVal);
            mParameters->changeParameter(name, prevVal);
        }
    }
}

/*! \brief Construct a common part of the slider query 
 * 
 */
void CinParamSliders::constructQueryString()
{
    mSliderQuery = "SELECT * FROM ";
    mSliderQuery += mCurDatabase->getTableName(); 
    mSliderQuery += " WHERE ";

    const QStringList &cols = mParameters->getParameterNames();
    for (int i=0;i<cols.count();i++)
    {
        mSliderQuery += cols.at(i) + "=:" + cols.at(i); 

        if (i < cols.count()-1)
        {
            mSliderQuery += " AND ";
        }

    }
}

void CinParamSliders::onParameterValueChanged(const QString &name, float value)
{
    // qDebug() << "CINPARAMSLIDERS: changed" << name << value;
    int count = mSliderLayout->rowCount();
    QLabel *label;
    for (int i=0;i<count;i++)
    {
        label = getLabelAt(i);
        if (label->text() == name)
        {
            getSliderAt(i)->setValue(value);
            break;
        }
    }
}

