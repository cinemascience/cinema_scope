#include "CinParamSliders.h"
#include "CinSlider.h"
#include <QDebug>
#include <QFileInfo>
#include <QLabel>
#include <QSlider>
#include <QVector>
#include <QSignalBlocker>

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
    qDebug() << "COLS: " << cols;
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

void CinParamSliders::deleteSliders()
{
    int count = mSliderLayout->rowCount();
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

    if (slider) {
        popSliderToValidValue(slider);
    } else {
        qWarning() << "ERROR: NULL CinSlider in signal/slot communication";
    }
}

// TODO this should only pop the current slider, not everything

/*! \brief Manage sliders so they pop to valid values in UI
 * 
 * Pop sliders to a valid value during drag: To overcome Qt's default behavior of sliders 
 * that increment step by 1. But our step is not always 1.
 */
void CinParamSliders::popSliderToValidValue(CinSlider *slider)
{
    // block signals for the duration of this call 
    const QSignalBlocker blocker(slider);

    float prevVal, nextVal;
    float curVal = slider->value();
    QString name = slider->getKey();
    bool bNext = mParameters->getNextValue(name, curVal, nextVal);
    bool bPrev = mParameters->getPrevValue(name, curVal, prevVal);

    if (not mParameters->valueExists(name, curVal))
    {
        if (bNext && bPrev) {
            if ( qAbs(prevVal - curVal) >= qAbs(nextVal - curVal) )
            {
                slider->setValue(nextVal);
                mParameters->changeParameter(name, nextVal);

            } else {
                slider->setValue(prevVal);
                mParameters->changeParameter(name, prevVal);
            }
        /*
        } else if (bNext) {
            slider->setValue(nextVal);
            mParameters->changeParameter(name, nextVal);
        } else if (bPrev) {
            slider->setValue(prevVal);
            mParameters->changeParameter(name, prevVal);
        } else {
            // what?
        */
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

