#include "CinSlider.h"
#include <QDebug>

CinSlider::CinSlider()
{
}

void CinSlider::setKey(QString &key)
{
    mKey = key;
    qDebug() << "KEY: " << key;
}
