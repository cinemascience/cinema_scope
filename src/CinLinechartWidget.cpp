#include "CinLinechartWidget.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QtCharts/QValueAxis>

CinLinechartWidget::~CinLinechartWidget()
{
}

CinLinechartWidget::CinLinechartWidget()
{
    QVBoxLayout *layout = new QVBoxLayout;
    QLineSeries *series = new QLineSeries;

    mChart.addSeries(series);
    mChart.setTitle("Emulated Results");

    QValueAxis *axisX = new QValueAxis;
    axisX->setLabelFormat("%i");
    axisX->setTitleText("Time");
    axisX->setMin(0.0);
    axisX->setMax(10.0);
    axisX->setTickCount(11);
    mChart.addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Value");
    axisY->setMin(0.0);
    axisY->setMax(10.0);
    axisY->setTickCount(11);
    mChart.addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    mChartView.setChart(&mChart);
    mChartView.setRenderHint(QPainter::Antialiasing);

    layout->addWidget(&mChartView);
    this->setLayout(layout);
}

