#ifndef CINLINECHARTWIDGET_H 
#define CINLINECHARTWIDGET_H 

#include <QWidget>
#include <QString>
#include <QMap>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

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

        void loadData(const QString &file);
        void loadSeries(const QString &name);

    private:

        QChart                       mChart;
        QChartView                   mChartView;
        QValueAxis                   mAxisX;
        QValueAxis                   mAxisY;
        QMap<QString,QString>        mData;
};

#endif // CINCOMPOUNDSLIDER_H
