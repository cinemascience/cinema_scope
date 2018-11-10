#ifndef CINDBSLIDERS_H
#define CINDBSLIDERS_H

#include <QWidget>
#include <QFormLayout>

namespace cin {

class DBSliders : public QWidget
{
public:
    DBSliders();
    void build();

private:
    void reset();

    QFormLayout    *mSliderLayout=NULL;
};

}

#endif // CINDBSLIDERS_H
