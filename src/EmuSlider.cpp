#include "EmuSlider.h"
#include <QDebug>

EmuSlider::~EmuSlider()
{
}

void EmuSlider::setRange(double min, double max)
{
    mMin = min;
    mMax = max;

    mValue.setText(QString::number(min));
}

void EmuSlider::setLabel(const QString &name)
{
    mLabel.setText(name);
}

EmuSlider::EmuSlider()
{
    mMin = 0.0;
    mMax = 0.0;
    mNumDivisions = 100;

    mLabel.setText("unset");
    // mLabel.setFrameStyle(QFrame::Panel | QFrame::Sunken);
    mLabel.setFixedSize(50, 20);
    mLabel.setAlignment(Qt::AlignLeft);

    mValue.setText("unset");
    mValue.setFrameStyle(QFrame::Panel | QFrame::Sunken);
    mValue.setFixedSize(100, 20);
    mValue.setAlignment(Qt::AlignLeft);

    mSlider.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    mSlider.setMinimum(0);
    mSlider.setMaximum(mNumDivisions);
    mSlider.setOrientation(Qt::Horizontal);
    mSlider.setTickInterval(1);
    mSlider.setTickPosition(QSlider::NoTicks);

    mLayout.addWidget(&mLabel);
    mLayout.addWidget(&mSlider);
    mLayout.addWidget(&mValue);
    mLayout.setContentsMargins(0,0,0,0);

    this->setLayout(&mLayout);

    connect(&mSlider, &QSlider::valueChanged, this, &EmuSlider::onSliderValueChanged);
}

void EmuSlider::onSliderValueChanged()
{
    QString stringValue;

    mValue.setText(QString::number(getValue()));
}

double EmuSlider::getValue()
{
    return mMin + ((double)mSlider.value()/(double)mNumDivisions)*(mMax-mMin);
}
