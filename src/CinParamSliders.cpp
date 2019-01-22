#include "CinParamSliders.h"
#include "CinCompoundSlider.h"
#include "CinParameter.h"
#include <QDebug>
#include <QFileInfo>
#include <QLabel>
#include <QSlider>
#include <QSpacerItem>
#include <QVector>
#include <QSignalBlocker>
#include <QLayoutItem>

CinParamSliders::CinParamSliders() 
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mSliderLayout = new QVBoxLayout(this);    
    // mSliderLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    setLayout(mSliderLayout);
}

void CinParamSliders::connect(CinDatabase *cdb, CinParamSet *params)
{
    if (cdb && params) 
    {
        mCurDatabase = cdb;
        mParameters  = params;
        
    } else {
        qWarning() << "ERROR: NULL database passed to CinParamSliders";
    }
}

/*! \brief Build the set of sliders
 *
 */
void CinParamSliders::buildSliders()
{
    CinCompoundSlider *compound = NULL;
    CinParameter *parameter     = NULL; 
    const QStringList &cols = mParameters->getParameterNames();
    for (int i=0;i<cols.count();i++)
    {
        compound  = new CinCompoundSlider(this);
        parameter = mParameters->getParameter(cols.at(i)); 
        compound->setParameter(parameter);
        mSliderLayout->addWidget(compound);
    }
    // add a spacer
    mSliderLayout->insertSpacerItem(-1, new QSpacerItem(10,10,QSizePolicy::Minimum, QSizePolicy::Expanding));
}

void CinParamSliders::deleteSliders()
{
    QLayoutItem *child = NULL;
    while ((child = mSliderLayout->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
}

void CinParamSliders::onParameterValueChanged(const QString &name, float value)
{
    CinParameter *param = mParameters->getParameter(name);
    if (param)
    {
        param->setValue(value);
    } else {
        qDebug() << "ERROR: param not found";
    }
}

