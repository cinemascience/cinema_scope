#include "EmuInputPanel.h"
#include "EmuSlider.h"
#include <QDebug>
#include <QSpacerItem>
#include <QLayoutItem>

EmuInputPanel::EmuInputPanel() 
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setLayout(&mLayout);


    QVector<double> minMax;
    for (int j=0;j<mEmulator.getNumInputs();j++)
    {
        mEmulator.getInputMinMax(j, minMax);
        mSliders.addSlider(QString("x%1").arg(j), minMax[0], minMax[1]);
    }
    mSliders.complete();

    mLayout.addWidget(&mSliders);
    mLayout.addWidget(&mEmulate);

    mEmulate.setText("emulate");
    connect(&mEmulate, &QPushButton::clicked, this, &EmuInputPanel::onEmulate);
}

void EmuInputPanel::setChart(CinLinechartWidget *chart)
{
    if (chart)
    {
        mChart = chart;
    } else {
        qDebug() << "ERROR: NULL chart";
    }
}

void EmuInputPanel::onEmulate()
{
    QVector<double> values;

    mSliders.getValues(values);
    mEmulator.emulate(values);
    mChart->addSeries(mEmulator.getResults());
}
