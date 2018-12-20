#include "CinCompoundSlider.h"
#include <QDebug>

CinCompoundSlider::CinCompoundSlider(QWidget *parent) : QWidget(parent)
{
    mValue.setText("new");

    mSlider.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    mSlider.setMinimum(0);
    mSlider.setMaximum(10);
    mSlider.setValue(1);
    mSlider.setOrientation(Qt::Horizontal);
    mSlider.setTickInterval(1);
    mSlider.setTickPosition(QSlider::TicksAbove);
    mLayout.addWidget(&mSlider);
    mLayout.addWidget(&mValue);
    this->setLayout(&mLayout);

    QObject::connect(&mSlider, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
}

void CinCompoundSlider::setKey(QString &key)
{
    mKey = key;
}

void CinCompoundSlider::onValueChanged(int value)
{
    QString stringValue;
    getValue(stringValue, value);

    mValue.setText(stringValue);
}

void CinCompoundSlider::getValue(QString &value, int id)
{
    value = QString::number(id);
}
