#ifndef CINDBSLIDERS_H
#define CINDBSLIDERS_H

#include <QWidget>
#include <QFormLayout>
#include <QSqlDatabase>

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
    void build(QSqlDatabase &database, QObject *receiver, const char *slotName);

private:
    void reset();

    QFormLayout *mSliderLayout=NULL; /*!< The layout object for the sliders */
    QStringList  mColNames;          /*!< TEMPORARY */

signals:
    void artifactSelected(QString &);
};


#endif // CINDBSLIDERS_H
