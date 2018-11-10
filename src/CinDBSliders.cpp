#include "CinDBSliders.h"
#include <QDebug>

using namespace cin;

DBSliders::DBSliders() 
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->mSliderLayout = new QFormLayout(this);    
    this->mSliderLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
}

void DBSliders::build()
{
}

void DBSliders::reset()
{
    // Remove all the rows. This deletes the children of the rows as well
    int count = this->mSliderLayout->rowCount();
    qDebug() << "SLIDER COUNT: " << count;
    for (int i=(count-1);i>=0;i--)
    {
        this->mSliderLayout->removeRow(i);
    }
}

