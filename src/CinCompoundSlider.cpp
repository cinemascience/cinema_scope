#include "CinCompoundSlider.h"
#include "CinParameter.h"
#include <QDebug>

CinCompoundSlider::~CinCompoundSlider()
{
    // everything else should be deleted automatically
    mParameter = NULL;
}

CinCompoundSlider::CinCompoundSlider(QWidget *parent) : QWidget(parent)
{
    mLabel.setText("unset");
    // mLabel.setFrameStyle(QFrame::Panel | QFrame::Sunken);
    mLabel.setFixedSize(50, 20);
    mLabel.setAlignment(Qt::AlignLeft);

    mValue.setText("unset");
    mValue.setFrameStyle(QFrame::Panel | QFrame::Sunken);
    mValue.setFixedSize(50, 20);
    mValue.setAlignment(Qt::AlignRight);

    mSlider.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    mSlider.setMinimum(0);
    mSlider.setMaximum(10);
    mSlider.setOrientation(Qt::Horizontal);
    mSlider.setTickInterval(1);
    mSlider.setTickPosition(QSlider::TicksAbove);

    mLayout.addWidget(&mLabel);
    mLayout.addWidget(&mSlider);
    mLayout.addWidget(&mValue);
    mLayout.setContentsMargins(0,0,0,0);

    this->setLayout(&mLayout);

    connect(&mSlider, &QSlider::valueChanged, this, &CinCompoundSlider::onSliderValueChanged);
}

void CinCompoundSlider::onSliderValueChanged(int value)
{
    QString stringValue;

    if (mParameter->getValueAsString(stringValue, value))
    {
        mValue.setText(stringValue);
        mParameter->setToValueAt(value);

        emit valueChanged(mParameter->getName(), stringValue);
    }
}

bool CinCompoundSlider::setParameter(CinParameter *p)
{
    bool result = false;

    if (p) 
    {
        mParameter = p;

        mLabel.setText(mParameter->getName());

        int numValues = mParameter->getNumValues();
        mSlider.setMinimum(0);
        mSlider.setMaximum(numValues-1);
        mSlider.setValue(0);

        QString stringValue;
        mParameter->getValueAsString(stringValue, 0);
        mValue.setText(stringValue);

        connect(mParameter, &CinParameter::valueChanged, 
                this, &CinCompoundSlider::onParameterValueChanged);

        result = true;
    } else {
        // TODO express warning
    }

    return result;
}

void CinCompoundSlider::onParameterValueChanged(const QString &value, int valueID)
{
    mSlider.setValue(valueID);
    mValue.setText(value);
    // qDebug() << "CINCOMPOUNDSLIDER onParameterValueChanged";
}
