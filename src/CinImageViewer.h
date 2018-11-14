#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QApplication>
#include <QtSql>
#include <QMainWindow>
#include <QSlider>
#include <QVBoxLayout>
#include <QLabel>
#include <QGraphicsScene>
#include <QPixmap>
#include <QGraphicsView>
#include <QDir>
#include <QFormLayout>
#include <QSqlField>
#include <QStyleFactory>
#include <QMouseEvent>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QPainter>
#include <QImage>
#include <QPaintEvent>
#include <QGraphicsScene>

#include <iostream>
#include <cstring>

#include "CinParamSet.h"

class MyImageView : public QGraphicsView
{
    Q_OBJECT

public:
    MyImageView(QWidget *parent) : QGraphicsView(parent) {}
    QGraphicsScene* sceneObj;
    CinParamSet *paramSet;

    int currentXloc;
    int currentYloc;
    int lastXloc=0;
    int lastYloc=0;
    const int slidePixel=50;
    bool mLoadImage(QString, QPixmap*);
    std::string constructQueryString(QStringList);
    QString mTableName;

    float currentPhi=0; //change later
    float currentTheta=30; //change later

protected:

    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent *);

};

#endif
