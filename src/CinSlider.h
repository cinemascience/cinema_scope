#ifndef CINSLIDER_H 
#define CINSLIDER_H 

#include <QSlider>
#include <QString>

//! A slider that knows the name of its parameter 
/*!
 *
*/
class CinSlider : public QSlider
{
    Q_OBJECT

    public:
        // constructor
        CinSlider();

        void setKey(QString &key) {mKey = key;}
        const QString &getKey() {return mKey;}

    private:
        QString mKey;
};

#endif // CINSLIDER_H
