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


#include <iostream>
#include <sstream>
#include <cstdlib>


using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QSqlDatabase, string, QWidget *parent = 0);
    vector<QSlider*> listOfSliders;
    vector<QLabel*> listOfSliderLabels;
    int numSliders;
    QGraphicsScene *scene;
    QGraphicsView *imageView;
    string rootPath;
    ~MainWindow();

private slots:

    void on_slider_valueChanged(int);
};

#endif // MAINWINDOW_H
