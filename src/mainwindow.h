#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include <QSplitter>

#include "CinDBReader.h"
#include "CinParamSliders.h"
#include "CinDatabase.h"
#include "CinParameter.h"
#include "CinParamSet.h"
#include "CinImageViewer.h"

#include <iostream>
#include <sstream>
#include <cstdlib>

using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void loadCinemaDatabase(const QString &database);
    int numSliders;
    ~MainWindow();

private:
    void createActions();
    void buildApplication(QWidget *parent);
    void flushUI();

    // actions
    QAction  *mOpenAction=NULL;
    QAction  *mQuitAction=NULL;
    QAction  *mAboutAction=NULL;

    QString   mCurDatabase;

    QSqlDatabase   *mDatabase=NULL;
    CinParamSet    *mParamSet=NULL;
    CinImageView   *mImageView=NULL;
    QWidget        *mImagePanel=NULL;
    QVBoxLayout    *mImageLayout=NULL;
    QFormLayout    *mSliderLayout=NULL;
    QGraphicsScene *mScene;
    QString         mTableName;
    CinDatabase    *mCDB=NULL;
    QSplitter   *splitter=NULL;

    // components


private slots:

    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;

    void onOpenFile();
    void onQuit();
    void onAbout();
};

#endif // MAINWINDOW_H
