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


#include <iostream>
#include <sstream>
#include <cstdlib>


using namespace std;

class MyImageView : public QGraphicsView
{
    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent*);
        void mouseMoveEvent(QGraphicsSceneMouseEvent*);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
};

class MyImageScene : public QGraphicsScene
{
    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent*);
        void mouseMoveEvent(QGraphicsSceneMouseEvent*);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QSqlDatabase, QString, QWidget *parent = 0);
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

    //void paintEvent(QPaintEvent *p) override;
    //QImage image = QImage("/home/soumya/Shared_Vbox/cinema_project_codes/cinema_scope/data/volume-render.cdb/images/1.jpg");


private:
    void createActions();
    void createMenus();

    // menus
    QMenuBar *mMenuBar=NULL;
    QMenu    *mFileMenu=NULL;
    QMenu    *mHelpMenu=NULL;
    // actions
    QAction  *mOpenAction=NULL;
    QAction  *mQuitAction=NULL;
    QAction  *mAboutAction=NULL;

    QString   mCurDatabase;

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
};

#endif // MAINWINDOW_H
