#ifndef EMUINPUTSLIDERS_H
#define EMUINPUTSLIDERS_H

#include <QWidget>
#include <QVBoxLayout>

/*! \brief Manages a set of input sliders
 *
*/
class EmuInputSliders : public QWidget
{
    Q_OBJECT

public:
    EmuInputSliders();

    void addSlider(const QString &, double, double);
    void deleteSliders();
    void complete();
    void getValues(QVector<double> &values);

private:
    QVBoxLayout mSliderLayout;
    
};
#endif // EMUINPUTSLIDERS_H
