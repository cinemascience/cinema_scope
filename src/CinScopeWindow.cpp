#include "CinScopeWindow.h"
#include <QObject>
#include <QSizePolicy>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QGraphicsSceneMouseEvent>
#include <QMessageBox>
#include <QSplitter>
#include <QSettings>
#include <QList>
#include <QVector>
#include "CinCore.h"
#include "CinDBReader.h"
#include "CinParamSliders.h"
#include "CinDatabase.h"
#include "CinParamSet.h"
#include "CinImageView.h"
#include "CinParameterMapDialog.h"
#include "CinLinechartWidget.h"
#include "EmuEmulator.h"

CinScopeWindow::~CinScopeWindow()
{
}

CinScopeWindow::CinScopeWindow(QWidget *parent) : QMainWindow(parent)
{
    // settings
    mSettingsFile = QDir::homePath() + "/.cinema/scope/settings.ini";
    // QSettings settings(mSettingsFile, QSettings::IniFormat);

    buildApplication(parent);
}

void CinScopeWindow::buildApplication(QWidget *parent)
{
    // build the DB and all its object
    mDBV = new CinDBView();
    CinDatabase *db = new CinDatabase();
    mDBV->setDatabase(db);

    // create the UI components
    QWidget     *mainWidget       = new QWidget(parent);
    QVBoxLayout *mainWidgetLayout = new QVBoxLayout;
    mSplitter  = new QSplitter(Qt::Horizontal, mainWidget);
    mSliders   = new CinParamSliders();
    mImageView = new CinImageView(mSplitter);
    mImageView->setParameters(mDBV->getParameters());

    mainWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainWidget->setLayout(mainWidgetLayout);
    // mainWidgetLayout->addWidget(mSplitter);

    // begin chart 
    CinLinechartWidget *chart = new CinLinechartWidget();
    mainWidgetLayout->addWidget(chart);
    chart->loadData("/Users/dhr/LANL/git/github/cinemascience/cinema_scope/unittesting/test_linechart/output.csv");
    // chart->loadSeries("1");
    // chart->loadSeries("10");
    // chart->loadSeries("100");
    // chart->loadSeries("1001");

    // emulate results and load
    EmuEmulator emu;
    QVector<double> emuInputs = ;
    emuInputs.reserve(5);
    emuInputs[0] = 0.002;
    emuInputs[1] = 0.002;
    emuInputs[2] = 0.01;
    emuInputs[3] = 0.4;
    emuInputs[4] = 0.5;
    emu.emulate(emuInputs);
    chart->addSeries(emu.getResults());

    mSplitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mSliders->connect(mDBV->getDatabase(), mDBV->getParameters());

    // set up initial state
    setCentralWidget(mainWidget);
    resize(900,600);
    createActions();
    setUnifiedTitleAndToolBarOnMac(true);
    menuBar()->setNativeMenuBar(false);

    // image and scene
    mScene = new QGraphicsScene();
    mImageView->setScene(mScene);
    mImageView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // mSplitter->addWidget(mImageView);
    // mSplitter->addWidget(mSliders);
    mSplitter->setSizes(QList<int>({INT_MAX, INT_MAX}));

    // connect slots
    connect(mDBV, &CinDBView::artifactChanged, mImageView, &CinImageView::onLoadImage);

}

void CinScopeWindow::loadCinemaDatabase(const QString &database)
{
    // remember this DB
    mCurDatabase = database;

    // clear the view
    mImageView->clear();

    // determine if there are phi, theta and FILE parameters, or map them
    mDBV->reset();
    mDBV->getDatabase()->load(database);
    mDBV->initializeAttributes();

    bool pBool = mDBV->parameterExists("phi");
    bool tBool = mDBV->parameterExists("theta");
    bool fBool = mDBV->artifactExists("FILE");
    if (pBool && tBool && fBool)
    {
        // we are fine 
        qDebug() << "DATABASE PASSES";
    } else {
        CinParameterMapDialog map(this);
        map.connect(mDBV, mImageView);
        map.setWindowTitle("Map Parameters Dialog");
        map.exec();
    }

    // clean up all UI components
    flushUI();

    // build new UI
    mSliders->buildSliders();
    mDBV->updateArtifacts();

    // manage state
    mParameterAction->setEnabled(true);
}


void CinScopeWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    mOpenAction = fileMenu->addAction(tr("&Open ..."), this, &CinScopeWindow::onOpenFile);
    mOpenAction->setShortcut(QKeySequence::Open);
    mOpenAction->setStatusTip(tr("Open a file"));

    fileMenu->addSeparator();

    mQuitAction = fileMenu->addAction(tr("E&xit"), this, &CinScopeWindow::onQuit);
    mQuitAction->setShortcut(tr("Ctrl+Q"));
    mQuitAction->setStatusTip(tr("Quit application"));

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    mParameterAction = editMenu->addAction(tr("Edit &Parameters ..."), this, &CinScopeWindow::onParameters);
    mParameterAction->setShortcut(tr("Ctrl+E"));
    mParameterAction->setEnabled(false);

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(tr("&About"), this, &CinScopeWindow::onAbout);
}

void CinScopeWindow::onOpenFile()
{
    QSettings settings(mSettingsFile, QSettings::IniFormat);
    QString dataDir = settings.value("cinemascope/data").toString(); 
    // qDebug() << "SETTINGS: " << dataDir;
    QString result = QFileDialog::getExistingDirectory(this,
                         tr("Open Cinema Database"), dataDir, 
                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    loadCinemaDatabase(result);
}

void CinScopeWindow::onQuit()
{
    QApplication::quit();
}

void CinScopeWindow::onAbout()
{
    QMessageBox::about(this, tr("CinemaScope"),
                       tr("Cinema Scope v0.1<br><br>Los Alamos National Laboratory<br>Copyright 2018"));
}

void CinScopeWindow::onParameters()
{
    CinParameterMapDialog map(this);
    map.connect(mDBV, mImageView);
    map.setWindowTitle("Map Parameters Dialog");
    map.exec();

    mDBV->updateArtifacts();
}

void CinScopeWindow::flushUI()
{
    mSliders->deleteSliders();
}
