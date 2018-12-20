#ifndef CINCOMPOUNDSLIDER_H 
#define CINCOMPOUNDSLIDER_H 

#include <QWidget>
#include <QString>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>

//! A slider that knows the name of its parameter 
/*!
 *
*/
class CinCompoundSlider : public QWidget
{
    Q_OBJECT

    public:
        // constructor
        CinCompoundSlider(QWidget *parent);

        void setKey(QString &key);
        const QString &getKey() {return mKey;}

    public slots:
        void onValueChanged(int value);

    private:
        void getValue(QString &value, int id);

        QString     mKey;
        QHBoxLayout mLayout;
        QSlider     mSlider;
        QLabel      mValue;
};

#endif // CINCOMPOUNDSLIDER_H
