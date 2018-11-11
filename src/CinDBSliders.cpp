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
    // TODO: put the table name somewhere better
    QString tableName = "cinema";
    QSqlRecord record = database.record(tableName);

    // 
    // build the set of sliders and add them to the layout
    // 
    QLabel  *label  = NULL;
    QSlider *slider = NULL;
    QString column;
    QSqlQuery query;
    for (int i=0;i<record.count();i++)
    {
        column = record.field(i).name();
        // qDebug() << "SLIDER COLUMN: " << column; 
        this->mColNames.push_back(column);
        // get the min and max values
        query.exec("SELECT MIN(" + column + ") , MAX(" + column + ") FROM " + tableName);
        query.first();

        // create label
        label = new QLabel();
        label->setText(column);

        // create slider
        slider = new QSlider(Qt::Horizontal);
        slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        slider->setRange(query.value(0).toFloat(), query.value(1).toFloat());
        QObject::connect(slider, SIGNAL(valueChanged(int)), parent, slotName); 
        // qDebug() << "SLIDER: " << column << query.value(0).toFloat() << query.value(1).toFloat(); 
        
        // add these to the layout
        this->mSliderLayout->addRow(label, slider);
    }
}

void DBSliders::reset()
{
    // Remove all the rows. This deletes the children of the rows as well
    int count = this->mSliderLayout->rowCount();
    // qDebug() << "SLIDER COUNT: " << count;
    for (int i=(count-1);i>=0;i--)
    {
        this->mSliderLayout->removeRow(i);
    }

    // delete all the slider names
    this->mColNames.clear();
}
