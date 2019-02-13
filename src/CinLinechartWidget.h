#ifndef CINLINECHARTWIDGET_H 
#define CINLINECHARTWIDGET_H 

#include <QWidget>
#include <QString>
#include <QMap>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

//! A widget that manages a line chart and its data 
/*!
 *  Multiple sources can control a parameter, so this widget
 *  has a pointer to its parameter, and uses slots and signals
 *  to control behavior.
*/
class CinLinechartWidget : public QWidget
{
    Q_OBJECT

    public:
        CinLinechartWidget();
        ~CinLinechartWidget();

    private:
        QMap<QString, QLineSeries *> mData;
        QChart                       mChart;
        QChartView                   mChartView;
};

#endif // CINCOMPOUNDSLIDER_H
