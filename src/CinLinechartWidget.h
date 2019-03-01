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
        void addSeries(const QVector<double> &series);

        int    getSeriesSize() {return mSeriesSize;}
        double getXMin() {return mXMin;}
        double getXMax() {return mXMax;}
        double getYMin() {return mYMin;}
        double getYMax() {return mYMax;}

    private:
        void addSeries(QLineSeries *series);
        void setSeriesSize(int size) {mSeriesSize = size;}
        void setXMinMax(double min, double max) {mXMin=min;mXMax=max;}
        void setYMinMax(double min, double max) {mYMin=min;mYMax=max;}
        int  getXTickCount() {return 10;} // TODO: make dynamic
        int  getYTickCount() {return 10;} // TODO: make dynamic

        QChart                      mChart;
        QChartView                  mChartView;
        QValueAxis                  mAxisX;
        QValueAxis                  mAxisY;
        QMap<QString,QString>       mData;
        double                      mXMin=0.0;
        double                      mXMax=0.0;
        double                      mYMin=0.0;
        double                      mYMax=0.0;
        int                         mSeriesSize=0;
};

#endif // CINCOMPOUNDSLIDER_H
