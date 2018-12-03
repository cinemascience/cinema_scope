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
#include <QPixmapCache>
#include <QFileInfo>

#include <iostream>
#include <cstring>

#include "CinParamSet.h"

//! Manages the imageviewer and interactions on it.
/*!
 *  This class contains everything it needs to show the right image given the current
 *  parameter values. It is also responsible for controlling the mouse drag event, which 
 *  is another way of interacting and changing the images while exploring the database.
*/

class CinImageView : public QGraphicsView
{
    Q_OBJECT

public:
    CinImageView(QWidget *parent) : QGraphicsView(parent)
    {
        mCurImage = "NONE";
    }
    QGraphicsScene* sceneObj;
    void setParameters(CinParamSet *paramset) {mParamSet = paramset;}

    int currentXloc;
    int currentYloc;
    int lastXloc=0;
    int lastYloc=0;
    const int slidePixelHor=20;
    const int slidePixelVer=20;
    bool loadImage(const QString &path);
    QString mTableName;

    float currentPhi;
    float currentTheta;
    QPixmapCache mCache;

public slots:
    // TODO discuss use of const
    void onLoadImage(const QString &key, const QString &path);

protected:

    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent * event);

    QString mCurImage; /* the current image */
    CinParamSet *mParamSet=NULL;
};

#endif
