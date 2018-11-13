#include "CinParamSliders.h"
#include <QDebug>
#include <QFileInfo>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QLabel>
#include <QSlider>
#include <QVector>

CinParamSliders::CinParamSliders() 
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->mSliderLayout = new QFormLayout(this);    
    this->mSliderLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    this->setLayout(this->mSliderLayout);
}

void CinParamSliders::setDatabase(CinDatabase *cdb)
{
    if (cdb) 
    {
        mCurDatabase = cdb;
        buildSliders();
    } else {
        qWarning() << "ERROR: NULL database passed to CinParamSliders";
    }
}

/*! \brief Build the set of sliders
 *
 */
void CinParamSliders::buildSliders()
{
    QSlider *slider = NULL;
    QString curColumn;
    QSqlQuery query;
    const QStringList &cols = mCurDatabase->getParameterColumnNames();
    for (int i=0;i<cols.count();i++)
    {
        // get the min and max values
        query.exec("SELECT MIN(" + cols.at(i) + ") , MAX(" + cols.at(i) + ") FROM " + mCurDatabase->getTableName());
        query.first();

        // create slider
        slider = new QSlider(Qt::Horizontal);
        slider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        slider->setRange(query.value(0).toFloat(), query.value(1).toFloat());
        QObject::connect(slider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged(int)));
        
        // add these to the layout
        this->mSliderLayout->addRow(cols.at(i), slider);
    }

    constructQueryString();
}

void CinParamSliders::reset()
{
    // Remove all the rows. This deletes the children of the rows as well
    int count = this->mSliderLayout->rowCount();
    // qDebug() << "SLIDER COUNT: " << count;
    for (int i=(count-1);i>=0;i--)
    {
        this->mSliderLayout->removeRow(i);
    }

    // clear the query
    this->mSliderQuery.clear();
}

QSlider *CinParamSliders::getSliderAt(int i)
{
    return dynamic_cast<QSlider *>(this->mSliderLayout->itemAt(i, QFormLayout::FieldRole)->widget());
}

QLabel *CinParamSliders::getLabelAt(int i)
{
    return dynamic_cast<QLabel *>(this->mSliderLayout->itemAt(i, QFormLayout::LabelRole)->widget());
}


/*! \brief Find the artifact defined by the current parameters 
 * 
 */
void CinParamSliders::onSliderValueChanged(int value)
{
    QSqlQuery query;

    query.prepare(QString::fromStdString(this->mSliderQuery.toStdString()));
    int numSliders = this->mCurDatabase->getNumParameterColumns();
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
        QString imagePath = mCurDatabase->getPath();
        imagePath += "/";
        imagePath += query.value(numSliders).toString(); 

        // if a file exists at this path, return it, otherwise, 
        // return the 'empty' image
        if ( ! (QFileInfo::exists(imagePath) && QFileInfo(imagePath).isFile()) )
        {
            imagePath = mCurDatabase->getPath();
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
void CinParamSliders::popSlidersToValidValue()
{
    QString minText, maxText;
    QSqlQuery minQuery, maxQuery;
    float minVal,maxVal;

    const QStringList &cols = mCurDatabase->getParameterColumnNames();
    int numSliders = mCurDatabase->getNumParameterColumns();
    QSlider *slider = NULL;
    for(int i=0;i<numSliders;i++)
    {
        slider = this->getSliderAt(i);

        minText  = QString("SELECT min(%1) FROM %2 WHERE %3 >= %4").arg(cols.at(i), mCurDatabase->getTableName(), cols.at(i), QString::number(slider->value()));
        minQuery.exec(minText);

        maxText  = QString("SELECT max(%1) FROM %2 WHERE %3 >= %4").arg(cols.at(i), mCurDatabase->getTableName(), cols.at(i), QString::number(slider->value()));
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
void CinParamSliders::constructQueryString()
{
    this->mSliderQuery = "SELECT * FROM ";
    this->mSliderQuery += mCurDatabase->getTableName(); 
    this->mSliderQuery += " WHERE ";

    const QStringList &cols = mCurDatabase->getParameterColumnNames();
    int numSliders = this->mCurDatabase->getNumParameterColumns();
    for(int i=0;i<numSliders;i++)
    {
        this->mSliderQuery += cols.at(i) + "=:" + cols.at(i); 

        if (i<numSliders-1)
        {
            this->mSliderQuery += " AND ";
        }

    }
}

