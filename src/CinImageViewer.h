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

#include <iostream>
#include <cstring>

#include "CinParamSet.h"

class CinImageView : public QGraphicsView
{
    Q_OBJECT

public:
    CinImageView(QWidget *parent) : QGraphicsView(parent) {}
    QGraphicsScene* sceneObj;
    CinParamSet *paramSet;

    int currentXloc;
    int currentYloc;
    int lastXloc=0;
    int lastYloc=0;
    const int slidePixel=20;
    bool loadImage(const QString &path);
    std::string constructQueryString(QStringList);
    QString mTableName;

    float currentPhi;
    float currentTheta;
    QString dbPath;
    QPixmapCache mCache;

public slots:
    // TODO discuss use of const
    void onLoadImage(const QString &key, const QString &path);

protected:

    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent * event);

};

#endif
