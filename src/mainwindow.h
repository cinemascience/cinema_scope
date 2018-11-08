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


#include <iostream>
#include <sstream>
#include <cstdlib>


using namespace std;

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
    QGraphicsView *imageView;
    string rootPath;
    ~MainWindow();

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

    void on_slider_valueChanged(int);
    string constructQueryString();
    void popSlidersOnValidValue();
    void mouseReleaseEvent(QMouseEvent *e);
    void onOpenFile();
    void onQuit();
    void onAbout();
};

#endif // MAINWINDOW_H
