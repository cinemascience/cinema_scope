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
    mAxisX.setTitleText("Run ID");
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
    int   numCols = 1000;
    float yMin    = 0.0;
    float yMax    = 210.0;

    mAxisX.setMin(0);
    mAxisX.setMax(numCols);
    mAxisX.setTickCount(10);

    mAxisY.setMin(yMin);
    mAxisY.setMax(yMax);
    mAxisY.setTickCount(10);

    // load the data into an internal data structure
    qDebug() << "FILE: " << path << file.exists();
    if (file.open(QIODevice::ReadOnly)) 
    {
        QTextStream stream(&file);
        QString line = stream.readLine();
        while (!line.isNull()) 
        {
            // use the first CSV value as the key, the rest as the value
            int id = line.indexOf(",");
            mData.insert(line.left(id), line.mid(id+1));

            line = stream.readLine();
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
        mChart.addSeries(series);

        series->attachAxis(&mAxisY);
        series->attachAxis(&mAxisX);
        series->setVisible(true);
    }
}
