#ifndef CINDBSLIDERS_H
#define CINDBSLIDERS_H

#include <QWidget>
#include <QFormLayout>
#include <QSqlDatabase>
#include <QSlider>
#include <QLabel>

/*! \brief This class manages a set of parameter sliders for a Cinema Database
 *
 * The class builds/rebuilds a set of sliders that select valid values
 * for a set of parameters in a Cinema Database. The set of paramaters
 * managed need not be the full set of parameters available in the database.
 * 
 * An instance of this class emits the following signals:
 *
 *    artifactSelected(QString &) 
*/
class DBSliders : public QWidget
{
    Q_OBJECT

public:
    DBSliders();
    void setDatabase(const QString &);
    void build(QSqlDatabase &database, QObject *receiver, const char *slotName);

private:
    void reset();
    void constructQueryString();
    void popSlidersToValidValue();
    QSlider *getSliderAt(int i);
    QLabel *getLabelAt(int i);

    QFormLayout *mSliderLayout=NULL; /*!< The layout object for the sliders */
    QString      mSliderQuery;       /*!< Common query over all sliders */
    
    QStringList  mColNames;          /*!< TEMPORARY column names */
    QString      mTableName;         /*!< TEMPORARY table name */
    QString      mCurDatabase;       /*!< TEMPORARY cinema database path */

private slots:
    void onSliderValueChanged(int value);

signals:
    void artifactSelected(QString &);
};


#endif // CINDBSLIDERS_H
