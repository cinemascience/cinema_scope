#ifndef EMUINPUTPANEL_H
#define EMUINPUTPANEL_H

#include <QPushButton>
#include <QScrollArea>
#include <QWidget>
#include <QVBoxLayout>
#include "CinLinechartWidget.h"
#include "EmuEmulator.h"
#include "EmuInputSliders.h"
#include "EmuDatabase.h"

/*! \brief Manages a set of input sliders
 *
*/
class EmuInputPanel : public QWidget
{
    Q_OBJECT

public:
    EmuInputPanel();

    void setChart(CinLinechartWidget *chart);
    void setDatabase(EmuDatabase *db) {mDatabase = db;}


private slots:
    void onEmulate();

private:
    QVBoxLayout         mLayout;
    EmuInputSliders     mSliders;
    QPushButton         mEmulate;
    EmuEmulator       mEmulator;
    EmuDatabase        *mDatabase=NULL;
    CinLinechartWidget *mChart=NULL;
    QScrollArea         mEmuList;
};
#endif // EMUINPUTPANEL_H
