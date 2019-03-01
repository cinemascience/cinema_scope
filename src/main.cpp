#include "CinScopeWindow.h"
#include "CinDBReader.h"
#include <QCoreApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QApplication cinemaViewer(argc, argv);
    cinemaViewer.setStyleSheet("QSlider {height:20px; width:50}");

    // 
    // handle command line args 
    //
    QCoreApplication::setApplicationName("EmuTracker");
    QCoreApplication::setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("EmuTracker v0.1");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("database", 
            QCoreApplication::translate("main", "Cinema Database to open"));

    parser.process(cinemaViewer);
    const QStringList args = parser.positionalArguments();

    CinScopeWindow mainWindow(NULL);
    mainWindow.setWindowTitle("EmuTracker");
    mainWindow.show();

    // currently, there is one positional argument
    if (args.size() > 0) 
    {
        mainWindow.loadCinemaDatabase(args.at(0));
    }

    return cinemaViewer.exec();
}
