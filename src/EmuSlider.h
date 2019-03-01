#ifndef EMUSLIDER_H 
#define EMUSLIDER_H 

#include <QWidget>
#include <QString>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>

//! A compound widget slider for a double value
/*!
*/
class EmuSlider : public QWidget
{
    Q_OBJECT

    public:
        EmuSlider();
        ~EmuSlider();

        void setRange(double min, double max);
        void setLabel(const QString &name);
        double getValue();

    public slots:
        void onSliderValueChanged(int value);

    private:
        QHBoxLayout     mLayout;
        QLabel          mLabel;
        QSlider         mSlider;
        QLabel          mValue;
        double          mMin;
        double          mMax;
        int             mNumDivisions;
};

#endif // EMUSLIDER_H
