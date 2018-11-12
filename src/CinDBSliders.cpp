#include "CinDBSliders.h"
#include <QDebug>
#include <QFileInfo>
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

void DBSliders::setDatabase(const QString &database)
{
    this->mCurDatabase = database;
}

bool DBSliders::isArtifactColumn(QString &name)
{
    bool ret = false;

    if ( name == "FILE" ) 
    {
        ret = true;
    }

    return ret;
}

void DBSliders::build(QSqlDatabase &database, QObject *parent, const char *slotName) 
{
    // TODO: put the table name somewhere better
    this->mTableName = "cinema";
    QSqlRecord record = database.record(this->mTableName);

    // 
    // build the set of sliders and add them to the layout
    // 
    QSlider *slider = NULL;
    QString column;
    QSqlQuery query;
    for (int i=0;i<record.count();i++)
    {
        column = record.field(i).name();

        if ( not isArtifactColumn(column) ) {
            this->mColNames.push_back(column);
            // get the min and max values
            query.exec("SELECT MIN(" + column + ") , MAX(" + column + ") FROM " + this->mTableName);
            query.first();

            // create slider
            slider = new QSlider(Qt::Horizontal);
            slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            slider->setRange(query.value(0).toFloat(), query.value(1).toFloat());
            QObject::connect(slider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged(int)));
            
            // add these to the layout
            this->mSliderLayout->addRow(column, slider);
        }
    }

    constructQueryString();
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

    // clear the query
    this->mSliderQuery.clear();
}

QSlider *DBSliders::getSliderAt(int i)
{
    return dynamic_cast<QSlider *>(this->mSliderLayout->itemAt(i, QFormLayout::FieldRole)->widget());
}

QLabel *DBSliders::getLabelAt(int i)
{
    return dynamic_cast<QLabel *>(this->mSliderLayout->itemAt(i, QFormLayout::LabelRole)->widget());
}


/*! \brief Find the artifact defined by the current parameters 
 * 
 */
void DBSliders::onSliderValueChanged(int value)
{
    QSqlQuery query;

    query.prepare(QString::fromStdString(this->mSliderQuery.toStdString()));
    int numSliders = this->mColNames.size();
    QString s;
    QSlider *slider = NULL;
    QSlider *label = NULL;
    for(int i=0;i<numSliders;i++)
    {
        s = ":" + this->getLabelAt(i)->text();
        query.bindValue(s, this->getSliderAt(i)->value());
    }
    query.exec();
    if (query.first())
    {
        //get the last column which has the image name
        QString imagePath = this->mCurDatabase;
        imagePath += "/";
        imagePath += query.value(numSliders).toString(); 

        // if a file exists at this path, return it, otherwise, 
        // return the 'empty' image
        if ( ! (QFileInfo::exists(imagePath) && QFileInfo(imagePath).isFile()) )
        {
            imagePath = this->mCurDatabase;
            imagePath += "/";
            imagePath += "empty_image/empty.png";
        }

        emit artifactSelected(imagePath);
        qDebug() << "DBSLIDER EMIT: " << imagePath;
    }

    popSlidersToValidValue();
}


/*! \brief Manage sliders so they pop to valid values in UI
 * 
 * Pop sliders to a valid value during drag: To overcome Qt's default behavior of sliders 
 * that increment step by 1. But our step is not always 1.
 */
void DBSliders::popSlidersToValidValue()
{
    QString minText, maxText;
    QSqlQuery minQuery, maxQuery;
    float minVal,maxVal;

    int numSliders = this->mColNames.size();
    QSlider *slider = NULL;
    for(int i=0;i<numSliders;i++)
    {
        slider = this->getSliderAt(i);

        minText  = QString("SELECT min(%1) FROM %2 WHERE %3 >= %4").arg(this->mColNames[i], this->mTableName, this->mColNames[i], QString::number(slider->value()));
        minQuery.exec(minText);

        maxText  = QString("SELECT max(%1) FROM %2 WHERE %3 >= %4").arg(this->mColNames[i], this->mTableName, this->mColNames[i], QString::number(slider->value()));
        maxQuery.exec(maxText);

        while (minQuery.next())
        {
            minVal = minQuery.value(0).toString().toFloat();
        }

        while (maxQuery.next())
        {
            maxVal = maxQuery.value(0).toString().toFloat();
        }

        if ( qAbs(minVal - static_cast<float>(slider->value())) >= qAbs(maxVal - static_cast<float>(slider->value())) )
        {
            slider->setValue(maxVal);
        } else {
            slider->setValue(minVal);
        }
    }
}

/*! \brief Construct a common part of the slider query 
 * 
 */
void DBSliders::constructQueryString()
{
    this->mSliderQuery = "SELECT * FROM ";
    this->mSliderQuery += this->mTableName;
    this->mSliderQuery += " WHERE ";

    int numSliders = this->mColNames.size();
    for(int i=0;i<numSliders;i++)
    {
        if (i<numSliders-1)
        {
            this->mSliderQuery += this->mColNames[i] + "=:" + this->mColNames[i] + " AND ";
        } else 
        {
            this->mSliderQuery += this->mColNames[i] + "=:" + this->mColNames[i];
        }

    }
}
