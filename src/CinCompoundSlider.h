#ifndef CINCOMPOUNDSLIDER_H 
#define CINCOMPOUNDSLIDER_H 

#include <QWidget>
#include <QString>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>

// forward declaration
class CinParameter;

//! A compound widget slider for a single Cinema parameter 
/*!
 *  Multiple sources can control a parameter, so this widget
 *  has a pointer to its parameter, and uses slots and signals
 *  to control behavior.
*/
class CinCompoundSlider : public QWidget
{
    Q_OBJECT

    public:
        CinCompoundSlider(QWidget *parent);
        ~CinCompoundSlider();

        bool setParameter(CinParameter *p);

    signals:
        void valueChanged(const QString &key, const QString& value);

    public slots:
        void onSliderValueChanged(int value);
        void onParameterValueChanged(const QString &value, int valueID);

    private:
        bool getValue(QString &value, int id);

        QHBoxLayout mLayout;

        QLabel      mLabel;
        QSlider     mSlider;
        QLabel      mValue;
        CinParameter *mParameter=NULL;
};

#endif // CINCOMPOUNDSLIDER_H
