#ifndef EMUINPUTPANEL_H
#define EMUINPUTPANEL_H

#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include "CinLinechartWidget.h"
#include "EmuEmulator.h"
#include "EmuInputSliders.h"

/*! \brief Manages a set of input sliders
 *
*/
class EmuInputPanel : public QWidget
{
    Q_OBJECT

public:
    EmuInputPanel();

    void setChart(CinLinechartWidget *chart);


private slots:
    void onEmulate();

private:
    QVBoxLayout         mLayout;
    EmuInputSliders     mSliders;
    QPushButton         mEmulate;
    EmuEmulator         mEmulator;
    CinLinechartWidget *mChart;
};
#endif // EMUINPUTPANEL_H
