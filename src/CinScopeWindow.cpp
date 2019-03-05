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
#include <time.h>
#include "CinCore.h"
#include "CinDBReader.h"
#include "CinParamSliders.h"
#include "CinDatabase.h"
#include "CinParamSet.h"
#include "CinImageView.h"
#include "CinParameterMapDialog.h"
#include "CinLinechartWidget.h"
#include "EmuInputPanel.h"

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
    mainWidgetLayout->addWidget(mSplitter);

    // Emulation UI 
    mSplitter->addWidget(&mEmuChart);
    mEmuChart.loadData("test.emu/output.csv");
    mSplitter->addWidget(&mEmuInput);
    mEmuInput.setChart(&mEmuChart);

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
    QMessageBox::about(this, tr("EmuTracker"),
                       tr("EmuTracker v0.1<br><br>Los Alamos National Laboratory<br>Copyright 2018"));
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
