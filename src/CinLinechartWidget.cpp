#include "CinLinechartWidget.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QListIterator>
#include <QVBoxLayout>

CinLinechartWidget::~CinLinechartWidget()
{
}

CinLinechartWidget::CinLinechartWidget()
{
    QVBoxLayout *layout = new QVBoxLayout;
    QLineSeries *series = new QLineSeries;

    mChart.addSeries(series);
    mChart.setTitle("Emulated Results");

    mAxisX.setLabelFormat("%i");
    mAxisX.setTitleText("Time");
    mAxisX.setMin(0.0);
    mAxisX.setMax(10.0);
    mAxisX.setTickCount(11);
    mChart.addAxis(&mAxisX, Qt::AlignBottom);
    series->attachAxis(&mAxisX);

    mAxisY.setLabelFormat("%i");
    mAxisY.setTitleText("Value");
    mAxisY.setMin(0.0);
    mAxisY.setMax(10.0);
    mAxisY.setTickCount(11);
    mChart.addAxis(&mAxisY, Qt::AlignLeft);
    series->attachAxis(&mAxisY);

    mChartView.setChart(&mChart);
    mChartView.setRenderHint(QPainter::Antialiasing);

    layout->addWidget(&mChartView);
    this->setLayout(layout);
}

void CinLinechartWidget::load(const QString &path)
{
    QString data;
    QFile file(path);

    // for the moment, assume that the json file is loaded
    int   numCols = 1000;
    float yMin    = 0.0;
    float yMax    = 210.0;
    // float yMax    = 0.01;

    mAxisX.setMin(0);
    mAxisX.setMax(numCols);
    mAxisX.setTickCount(10);

    mAxisY.setMin(yMin);
    mAxisY.setMax(yMax);
    mAxisY.setTickCount(10);

    qDebug() << "FILE: " << path << file.exists();
    if (file.open(QIODevice::ReadOnly)) 
    {
        QLineSeries *series = NULL;
        QTextStream stream(&file);
        QString line = stream.readLine();
        QStringList values;
        int set = 0;
        while (!line.isNull()) 
        {
            int cur = 0;
            series = new QLineSeries;
            values = line.split(",");
            QListIterator<QString> it(values);
            float value = 0.0;
            // swallow the first value
            it.next();
            while (it.hasNext())
            {
                value = QString(it.next().toLocal8Bit().constData()).toFloat();
                series->append((float)cur, value); 
                // qDebug() << value << endl; 
                cur++;
                // if (cur > 200) 
                // {
                    // break;
                // } 
            }
            qDebug() << "loaded";
            mChart.addSeries(series);
            qDebug() << "added";

            // get the next line
            line = stream.readLine();

            series->attachAxis(&mAxisY);
            series->attachAxis(&mAxisX);
            qDebug() << "attached";

            // debug
            set++;
            qDebug() << "set: " << set;

            if (set > 50)
            {
                break;
            }
        }
    } else {
        qDebug() << "COULD NOT OPEN FILE: " << path;
    }
}
