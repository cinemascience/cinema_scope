#ifndef CINDBSLIDERS_H
#define CINDBSLIDERS_H

#include <QWidget>
#include <QFormLayout>
#include <QSqlDatabase>

class DBSliders : public QWidget
{
    Q_OBJECT

public:
    DBSliders();
    void build(QSqlDatabase &database, QObject *receiver, const char *slotName);

private:
    void reset();

    QFormLayout    *mSliderLayout=NULL;

};


#endif // CINDBSLIDERS_H
