#include "CinParamSliders.h"
#include "CinSlider.h"
#include <QDebug>
#include <QFileInfo>
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
    CinSlider *slider = NULL;
    QString curColumn;
    const QStringList &cols = mParameters->getParameterNames();
    float min, max;
    for (int i=0;i<cols.count();i++)
    {
        if ( mParameters->getMinMax( cols.at(i), min, max ) )
        {
            // create slider
            slider = new CinSlider();
            slider->setOrientation(Qt::Horizontal);
            slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            slider->setRange(min, max);
            slider->setKey(const_cast<QString &>(cols.at(i)));
            QObject::connect(slider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged(int)));
        
            // add these to the layout
            mSliderLayout->addRow(cols.at(i), slider);

        } else {
            qWarning() << "ERROR: failed to get min/max values from parameter set"; 
        }

    }
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
    CinSlider *slider = qobject_cast<CinSlider *>(QObject::sender());

    popSliderToValidValue(slider);
}

// TODO this should only pop the current slider, not everything

/*! \brief Manage sliders so they pop to valid values in UI
 * 
 * Pop sliders to a valid value during drag: To overcome Qt's default behavior of sliders 
 * that increment step by 1. But our step is not always 1.
 */
void CinParamSliders::popSliderToValidValue(CinSlider *slider)
{
    float prevVal, nextVal;
    float curVal = slider->value();
    QString name = slider->getKey();
    mParameters->getNextValue(name, curVal, nextVal);
    mParameters->getPrevValue(name, curVal, prevVal);

    if ( qAbs(prevVal - curVal) >= qAbs(nextVal - curVal) )
    {
        slider->setValue(nextVal);
        mParameters->changeParameter(name, nextVal);

    } else {
        slider->setValue(prevVal);
        mParameters->changeParameter(name, prevVal);
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

