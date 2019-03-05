#include "EmuInputPanel.h"
#include "EmuSlider.h"
#include <QDebug>
#include <QLayoutItem>
#include <QGridLayout>

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

    mLayout.addWidget(&mSliders);

    QGridLayout *grid = new QGridLayout();
    mLayout.addLayout(grid);
    mEmulate.setFixedSize(100,30);
    grid->addWidget(&mEmulate,0,1,1,1);
    grid->setColumnStretch(0,1);

    mEmuList.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mLayout.addWidget(&mEmuList);

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
    QVector<double> inputValues;

    mSliders.getValues(inputValues);
    mEmulator.emulate(inputValues);

    mChart->addSeries(mEmulator.getResults());
}
