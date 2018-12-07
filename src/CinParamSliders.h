#ifndef CINDBSLIDERS_H
#define CINDBSLIDERS_H

#include <QWidget>
#include <QFormLayout>
#include <QSqlDatabase>
#include <QSlider>
#include <QLabel>
#include "CinDatabase.h"
#include "CinParamSet.h"
#include "CinSlider.h"

/*! \brief Manages a set of parameter sliders for a Cinema Database
 *
 * The class builds/rebuilds a set of sliders that select valid values
 * for a set of parameters in a Cinema Database. The set of paramaters
 * managed need not be the full set of parameters available in the database.
 * 
*/
class CinParamSliders : public QWidget
{
    Q_OBJECT

public:
    CinParamSliders();
    void connect(CinDatabase *cdb, CinParamSet *params);
    void deleteSliders();
    void buildSliders();

private:
    void     popSliderToValidValue(CinSlider *slider);
    QSlider *getSliderAt(int i);
    QLabel  *getLabelAt(int i);

    QFormLayout *mSliderLayout=NULL; /*!< The layout object for the sliders */
    CinDatabase *mCurDatabase=NULL;  /*!< Common database object */ 
    CinParamSet *mParameters=NULL;
    
private slots:
    void onSliderValueChanged(int value);
    void onParameterValueChanged(const QString &name, float value);
};


#endif // CINDBSLIDERS_H
