#include "mainwindow.h"
#include <QApplication>

// Path of the CDB folder upto images folder
string dataPath = "/home/soumya/Shared_Vbox/cinema_project_codes/qt_cinemaDB_viewer/data/volume-render/images/";

int main(int argc, char *argv[])
{
    QApplication cinemaViewer(argc, argv);
    cinemaViewer.setStyleSheet("QSlider {height:20px; width:230px}");

    MainWindow mainWindow(3,dataPath);
    mainWindow.setWindowTitle("Qt-based CinemaDB Viewer");
    mainWindow.show();

    return cinemaViewer.exec();
}
