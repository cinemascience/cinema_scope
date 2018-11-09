#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QtSql>
#include <QMainWindow>
#include <QSlider>
#include <QHBoxLayout>
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
#include "CinDBReader.h"


#include <iostream>
#include <sstream>
#include <cstdlib>


using namespace std;
using namespace cin;

class MyImageView : public QGraphicsView
{
    Q_OBJECT

public:
    MyImageView(QWidget *parent) : QGraphicsView(parent) {}

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent*);
    void mouseMoveEvent(QGraphicsSceneMouseEvent*);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
    void mouseMoveEvent(QMouseEvent *);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void loadCinemaDatabase(const QString &database);

    vector<QSlider*> listOfSliders;
    vector<QLabel*> listOfSliderLabels;
    vector<string> columnNames;
    QString tname;
    int numSliders;
    QGraphicsScene *scene;
    //MyImageScene *scene;
    //QGraphicsView *imageView;
    MyImageView *imageView;
    string rootPath;
    ~MainWindow();

    //virtual bool eventFilter(QObject *, QEvent *);

private:
    void createActions();
    void buildApplication(QWidget *parent);

    // actions
    QAction  *mOpenAction=NULL;
    QAction  *mQuitAction=NULL;
    QAction  *mAboutAction=NULL;

    QString   mCurDatabase;

    QSqlDatabase  mDatabase;
    MyImageView  *mImageView=NULL;
    QWidget      *mImagePanel=NULL;
    QHBoxLayout  *mImageLayout=NULL;
    QFormLayout  *mSliderLayout=NULL;
    DBReader     *mReader=NULL;

    // components


private slots:

    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;


    void on_slider_valueChanged(int);
    string constructQueryString();
    void popSlidersOnValidValue();
    void onOpenFile();
    void onQuit();
    void onAbout();

    //void paintEvent(QPaintEvent *p) override;
    //QImage image = QImage("/home/soumya/Shared_Vbox/cinema_project_codes/cinema_scope/data/volume-render.cdb/images/1.jpg");
};

#endif // MAINWINDOW_H
