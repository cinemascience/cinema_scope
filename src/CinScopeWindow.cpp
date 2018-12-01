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
#include "CinCore.h"
#include "CinDBReader.h"
#include "CinParamSliders.h"
#include "CinDatabase.h"
#include "CinParameter.h"
#include "CinParamSet.h"
#include "CinImageViewer.h"
#include "CinDBFactory.h"

CinScopeWindow::~CinScopeWindow()
{
}

void CinScopeWindow::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        cout<<"On left mouse release"<<endl;
    }
}

void CinScopeWindow::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        cout<<"On left mouse press"<<endl;
    }
}

void CinScopeWindow::mouseMoveEvent(QMouseEvent *e)
{
    QPoint p = e->pos();
    cout<<"MAINWINDOW: moving mouse loc: "<< p.rx()<<" "<<p.ry()<<endl;
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
    mDBV = CinDBFactory::BuildDBView();

    // remember the table name
    mTableName = "cinema";

    // create the basic components
    QWidget     *mainWidget       = new QWidget(parent);
    QVBoxLayout *mainWidgetLayout = new QVBoxLayout;
    mSplitter  = new QSplitter(Qt::Horizontal, mainWidget);
    mSliders   = new CinParamSliders();
    mImageView = new CinImageView(mSplitter);

    mainWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainWidget->setLayout(mainWidgetLayout);
    mainWidgetLayout->addWidget(mSplitter);

    mSplitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // set up initial state
    setCentralWidget(mainWidget);
    resize(900,600);
    createActions();
    setUnifiedTitleAndToolBarOnMac(true);
    menuBar()->setNativeMenuBar(false);

    // image and scene
    mScene = new QGraphicsScene();
    mImageView->sceneObj = mScene;
    mImageView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mImageView->mTableName = mTableName;
    mSplitter->addWidget(mImageView);
    mSplitter->addWidget(mSliders);
    mSplitter->setSizes(QList<int>({INT_MAX, INT_MAX}));
}

void CinScopeWindow::loadCinemaDatabase(const QString &database)
{
    // clean up all UI components
    flushUI();

    // remember this DB
    mCurDatabase = database;
    mImageView->dbPath = database;

    mDBV->load(database, mTableName);
    mSliders->connect(mDBV->getDatabase(), mDBV->getParameters());
    // QObject::connect(mSliders, SIGNAL(artifactSelected(QString &)), mImageView, SLOT(onLoadImage(QString &)));
    QObject::connect(mDBV,       SIGNAL(artifactChanged(const QString &, const QString &)), 
                     mImageView, SLOT(onLoadImage(const QString &, const QString &)));
    // Testing query string
    // QString tempQuery;
    // mDBV->getParameters()->getArtifactQueryString(tempQuery);
    // qDebug() << "QUERY: " << tempQuery;
    // mDBV->updateArtifacts();

    mImageView->paramSet = mDBV->getParameters();

    QSqlQuery qry;
    string queryText = "SELECT * FROM " + mTableName.toStdString();
    qry.exec(queryText.c_str());

    //Get number of sliders = number of columns-1
    numSliders = qry.record().count()-1;
    // qDebug() << "Number of columns: " << numSliders + 1 << endl;

    //load and display an initial image
    queryText = "SELECT * FROM " + mTableName.toStdString();
    qry.exec(queryText.c_str());
    QString initFileID;
    qry.first();
    //get the value of last column which is the image path
    initFileID = qry.value(numSliders).toString();

    QString imagePath = database;
    imagePath += "/" + initFileID; //loads the first image from first row in the db
    // QPixmap image;
    // if (!mImageView->loadImage(imagePath,&image))
    if (!mImageView->loadImage(imagePath))
    {
        //image.fill(Qt::transparent); // shows a blank screen
        qWarning() << "image loading failed";
        imagePath = database;
        imagePath += "/empty_image/empty.png";
        // mImageView->loadImage(imagePath,&image);
        mImageView->loadImage(imagePath);
    }

    // mScene->addPixmap(image);
    mImageView->setScene(mScene);
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
                       tr("This is the about message"));
}

void CinScopeWindow::flushUI()
{
    // flush the database
    QSqlQuery qry;
    qry.exec("DROP TABLE " + mTableName);
}
