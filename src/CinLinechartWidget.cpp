#include "CinLinechartWidget.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QListIterator>
#include <QVBoxLayout>
#include <QLegendMarker>

CinLinechartWidget::~CinLinechartWidget()
{
}

CinLinechartWidget::CinLinechartWidget()
{
    QVBoxLayout *layout = new QVBoxLayout;

    mChart.setTitle("Emulated Results");

    mAxisX.setLabelFormat("%i");
    mAxisX.setTitleText("Time ID");
    mAxisX.setMin(0.0);
    mAxisX.setMax(10.0);
    mAxisX.setTickCount(11);
    mChart.addAxis(&mAxisX, Qt::AlignBottom);

    mAxisY.setLabelFormat("%i");
    mAxisY.setTitleText("Value");
    mAxisY.setMin(0.0);
    mAxisY.setMax(10.0);
    mAxisY.setTickCount(11);
    mChart.addAxis(&mAxisY, Qt::AlignLeft);

    mChartView.setChart(&mChart);
    mChartView.setRenderHint(QPainter::Antialiasing);


    layout->addWidget(&mChartView);
    this->setLayout(layout);
}

void CinLinechartWidget::loadData(const QString &path)
{
    QString data;
    QFile file(path);

    // for the moment, assume that the json file is loaded
    setSeriesSize(1000);
    setYMinMax(0.0, 210.0);
    setXMinMax(0.0, getSeriesSize());

    mAxisX.setMin(getXMin());
    mAxisX.setMax(getXMax());
    mAxisX.setTickCount(getXTickCount());

    mAxisY.setMin(getYMin());
    mAxisY.setMax(getYMax());
    mAxisY.setTickCount(getYTickCount());

    // load the data into an internal data structure
    qDebug() << "FILE: " << path << file.exists();
    if (file.open(QIODevice::ReadOnly)) 
    {
        QTextStream stream(&file);
        QString line = stream.readLine();

        // skip past the first line, as that's the one with the var names
        line = stream.readLine();
        int cur = 0;
        while (!line.isNull()) 
        {
            // use the first CSV value as the key, the rest as the value
            // int id = line.indexOf(",");
            // mData.insert(line.left(id), line.mid(id+1));
            mData.insert(QString::number(cur), line);

            line = stream.readLine();
            cur++;
        }

    } else {
        qDebug() << "COULD NOT OPEN FILE: " << path;
    }
}

void CinLinechartWidget::loadSeries(const QString &key)
{
    QString value = mData.value(key, "ERROR");
    // qDebug() << "KEY: " << key;
    // qDebug() << "VAL: " << value;

    if (value != "ERROR") 
    {
        QLineSeries *series = new QLineSeries;
        QStringList values = value.split(",");
        QListIterator<QString> it(values);
        float value = 0.0;
        float cur   = 0.0;
        while (it.hasNext())
        {
            value = QString(it.next().toLocal8Bit().constData()).toFloat();
            series->append(cur, value); 
            cur++;
        }
        addSeries(series);
    }
}

void CinLinechartWidget::addSeries(const QVector<double> &series)
{
    QLineSeries *lineSeries = new QLineSeries;
    QVectorIterator<double> it(series);

    double cur = 0.0;
    double dValue = 0.0;
    while (it.hasNext())
    {
        dValue = it.next();
        lineSeries->append(cur, dValue);
        cur++;
    }

    mChart.addSeries(lineSeries);
}

void CinLinechartWidget::addSeries(QLineSeries *series)
{
    if (series)
    {
        mChart.addSeries(series);
        // series->attachAxis(&mAxisY);
        // series->attachAxis(&mAxisX);
        // series->setVisible(true);
    }
}
