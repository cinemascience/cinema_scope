#include "EmuInputSliders.h"
#include "EmuSlider.h"
#include <QDebug>
#include <QSpacerItem>
#include <QLayoutItem>

EmuInputSliders::EmuInputSliders() 
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setLayout(&mSliderLayout);
}

/*! \brief add a slider 
 *
 */
void EmuInputSliders::addSlider(const QString &name, double min, double max)
{
    EmuSlider *slider = new EmuSlider(); 
    slider->setRange(min, max);
    slider->setLabel(name);
    mSliderLayout.addWidget(slider);
}

void EmuInputSliders::deleteSliders()
{
    QLayoutItem *child = NULL;
    while ((child = mSliderLayout.takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
}

void EmuInputSliders::getValues(QVector<double> &values)
{
    QVector<double> curValues;

    int numSliders = mSliderLayout.count();
    for (int i=0;i<numSliders;i++)
    {
        EmuSlider *curSlider = 
                qobject_cast<EmuSlider *>(mSliderLayout.itemAt(i)->widget());
        curValues.append(curSlider->getValue());
    }

    values = curValues;
}
 
