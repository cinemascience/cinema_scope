#include "mainwindow.h"
#include <QApplication>

// Path of the CDB folder upto images folder
string dataPath = "/Users/dhr/LANL/git/github/cinemascience/cinema_scope/data/volume-render/images/";

int main(int argc, char *argv[])
{
    QApplication cinemaViewer(argc, argv);
    cinemaViewer.setStyleSheet("QSlider {height:20px; width:230px}");

    MainWindow mainWindow(3,dataPath);
    mainWindow.setWindowTitle("Qt-based CinemaDB Viewer");
    mainWindow.show();

    return cinemaViewer.exec();
}
