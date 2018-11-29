#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QSlider>
#include <QVBoxLayout>
#include <QLabel>
#include <QGraphicsScene>
#include <QPixmap>
#include <QGraphicsView>
#include <QDir>
#include <QFormLayout>
#include <QStyleFactory>
#include <QMouseEvent>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QPainter>
#include <QImage>
#include <QPaintEvent>
#include <QGraphicsScene>
#include <QSplitter>

#include "CinDBReader.h"
#include "CinParamSliders.h"
#include "CinDatabase.h"
#include "CinParameter.h"
#include "CinParamSet.h"
#include "CinImageViewer.h"
#include "CinDBView.h"

#include <iostream>
#include <sstream>
#include <cstdlib>

using namespace std;

class CinScopeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CinScopeWindow(QWidget *parent = 0);
    void loadCinemaDatabase(const QString &database);
    int numSliders;
    ~CinScopeWindow();

private:
    void createActions();
    void buildApplication(QWidget *parent);
    void flushUI();

    // actions
    QAction  *mOpenAction=NULL;
    QAction  *mQuitAction=NULL;
    QAction  *mAboutAction=NULL;

    QString          mCurDatabase;
    CinDBView       *mDBV=NULL;
    CinImageView    *mImageView=NULL;
    QGraphicsScene  *mScene;
    QSplitter       *mSplitter=NULL;
    CinParamSliders *mSliders=NULL;
    QString          mTableName;
    QString          mSettingsFile;

private slots:

    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;

    void onOpenFile();
    void onQuit();
    void onAbout();
};

#endif // MAINWINDOW_H
