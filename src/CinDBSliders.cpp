#include "CinDBSliders.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QLabel>
#include <QSlider>
#include <QVector>

DBSliders::DBSliders() 
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->mSliderLayout = new QFormLayout(this);    
    this->mSliderLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    this->setLayout(this->mSliderLayout);
}

void DBSliders::build(QSqlDatabase &database, QObject *parent, const char *slotName) 
{
    QString tableName = "cinema";
    QSqlRecord record = database.record(tableName);
    QStringList colNames;

    qDebug() << "SLOT: " << parent << slotName;

    QVector<float> minVals;
    QVector<float> maxVals;
    QString queryText;
    QSqlQuery query;
    QString column;
    for(int i=0;i<record.count();i++) //since last column is image file names
    {
        column = record.field(i).name();
        queryText = "SELECT MIN(" + column + ") , MAX(" + column + ") FROM " + tableName;
        query.exec(queryText);
        while (query.next())
        {
            minVals.push_back(query.value(0).toFloat());
            maxVals.push_back(query.value(1).toFloat());
        }
    }

    QLabel  *label  = NULL;
    QSlider *slider = NULL;
    for (int i=0;i<record.count();i++)
    {
        qDebug() << "SLIDER COLUMN: " << record.field(i).name();
        label = new QLabel();
        label->setText(record.field(i).name());
        slider = new QSlider(Qt::Horizontal);
        slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        slider->setRange(minVals.at(i), maxVals.at(i));
        QObject::connect(slider, SIGNAL(valueChanged(int)), parent, slotName); 
        this->mSliderLayout->addRow(label, slider);
        // qDebug() << "SLIDER: " << record.field(i).name() << minVals.at(i) << maxVals.at(i);
    }
}

void DBSliders::reset()
{
    // Remove all the rows. This deletes the children of the rows as well
    int count = this->mSliderLayout->rowCount();
    qDebug() << "SLIDER COUNT: " << count;
    for (int i=(count-1);i>=0;i--)
    {
        this->mSliderLayout->removeRow(i);
    }
}

