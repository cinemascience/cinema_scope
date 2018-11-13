#include "mainwindow.h"
#include <QSizePolicy>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QGraphicsSceneMouseEvent>
#include <QMessageBox>
#include <QSplitter>
#include "CinDBReader.h"
#include "CinParamSliders.h"
#include "CinDatabase.h"

MainWindow::~MainWindow()
{

}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        cout<<"On left mouse release"<<endl;
    }
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        cout<<"On left mouse press"<<endl;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    QPoint p = e->pos();
    cout<<"MAINWINDOW: moving mouse loc: "<< p.rx()<<" "<<p.ry()<<endl;
}

void MyImageView::mouseMoveEvent(QMouseEvent *e)
{
    QPoint p = e->pos();
    cout<<"MYIMAGE: moving mouse loc: "<< p.rx()<<" "<<p.ry()<<endl;

    string path = "../data/volume-render.cdb";

    string imagePath = path + "/" + "images/163.jpg";

    QPixmap image;
    bool loadSuccess = image.load(QString::fromStdString(imagePath));
    if(!loadSuccess)
    {
        cout<<"image loading failed!!"<<endl;
    }
    else
    {
        this->sceneObj->addPixmap(image);
    }
}

void MyImageView::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        cout<<"MYIMAGE: n left mouse release"<<endl;
        lastXloc = e->pos().rx();
        lastYloc = e->pos().ry();
    }
}

void MyImageView :: mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        cout<<"MYIMAGE: on left mouse press"<<endl;
        currentXloc = e->pos().rx();
        currentYloc = e->pos().ry();
    }
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->buildApplication(parent);
}

void MainWindow::buildApplication(QWidget *parent)
{
    // create the database and cinema reaader
    this->mCDB = new CinDatabase();
    this->mDatabase = this->mCDB->TEMPGetDatabase();
    // this->mDatabase = QSqlDatabase::addDatabase("QSQLITE");
    // this->mDatabase.open();
    // this->mReader = new CinDBReader();

    // remember the table name
    this->mTableName = "cinema";

    // create the basic components
    QWidget     *mainWidget       = new QWidget(parent);
    QVBoxLayout *mainWidgetLayout = new QVBoxLayout;
    QSplitter   *splitter         = new QSplitter(Qt::Horizontal, mainWidget);
    QWidget     *sliderPanel      = new QWidget();
    this->mImagePanel   = new QWidget();
    this->mImageLayout  = new QVBoxLayout;
    this->mSliderLayout = new QFormLayout;

    mainWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainWidget->setLayout(mainWidgetLayout);
    mainWidgetLayout->addWidget(splitter);

    splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    splitter->addWidget(this->mImagePanel);
    splitter->addWidget(sliderPanel);

    this->mImagePanel->setLayout(this->mImageLayout);
    this->mImagePanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    sliderPanel->setLayout(this->mSliderLayout);
    sliderPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // colors for testing
    // mainWidget->setStyleSheet("background-color:red");
    // splitter->setStyleSheet("background-color:blue");

    // set up initial state
    this->setCentralWidget(mainWidget);
    this->resize(1200,600);
    this->createActions();
    setUnifiedTitleAndToolBarOnMac(true);
    menuBar()->setNativeMenuBar(false);

    // image and scene
    this->mScene = new QGraphicsScene();
    this->mImageView = new MyImageView(this->mImagePanel);
    this->mImageView->sceneObj = this->mScene;
    this->mImageView->iListOfSliders = this->mListOfSliders;
    this->mImageView->iNumSliders = this->numSliders;
    this->mImageView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void MainWindow::loadCinemaDatabase(const QString &database)
{
    // clean up all UI components
    this->flushUI();

    // remember this DB
    this->mCurDatabase = database;

    // Cin* class testing
    mCDB->loadDatabase(database, this->mTableName);
    CinParamSliders *dbSliders = new CinParamSliders();
    dbSliders->setDatabase(mCDB);
    this->mImageLayout->addWidget(dbSliders);

    // load database

    mReader->readCinemaDatabase(this->mDatabase, database, this->mTableName);
    // mDatabase = mCinDatabase->TEMPGetDatabase();
    // mReader->readCinemaDatabase(this->mDatabase, database, this->mTableName);
    // mDatabase = mCinDatabase->TEMPGetDatabase(); 

    QSqlQuery qry;
    string queryText = "SELECT * FROM " + this->mTableName.toStdString();
    qry.exec(queryText.c_str());

    //Get number of sliders = number of columns-1
    this->numSliders = qry.record().count()-1;
    cout<<"Number of columns: "<<this->numSliders+1<<endl;

    //Get column names
    for(int i=0;i<qry.record().count();i++)
    {
        this->mColumnNames.push_back(this->mDatabase->driver()->record(this->mTableName).fieldName(i).toStdString());
    }

    //Get value ranges of each column
    vector<float> minVals;
    vector<float> maxVals;
    for(int i=0;i<this->numSliders;i++) //since last column is image file names
    {
        queryText = "SELECT MIN(" + this->mColumnNames[i] + ") , MAX(" + this->mColumnNames[i] + ") FROM " +this->mTableName.toStdString();
        qry.exec(queryText.c_str());
        while (qry.next())
        {
            minVals.push_back(qry.value(0).toFloat());
            maxVals.push_back(qry.value(1).toFloat());
            cout<<"Range of column "<<this->mColumnNames[i]<<" is: "<<minVals[i]<<" "<<maxVals[i]<<endl;
        }
    }

    // slider section
    this->mListOfSliders.resize(this->numSliders);
    this->mListOfSliderLabels.resize(this->numSliders);
    for(int i=0;i<this->numSliders;i++)
    {
        this->mListOfSliders[i] = new QSlider(Qt::Horizontal);
        this->mListOfSliders[i]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
        this->mListOfSliders[i]->setRange(minVals[i],maxVals[i]);
        //this->mListOfSliders[i]->setSingleStep(11);
        QObject::connect(this->mListOfSliders[i],SIGNAL(valueChanged(int)),this,SLOT(on_slider_valueChanged(int)));
        stringstream ss;
        ss<<i;
        string label = this->mColumnNames[i];
        this->mListOfSliderLabels[i] = new QLabel;
        this->mListOfSliderLabels[i]->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        this->mListOfSliderLabels[i]->setText(label.c_str());
    }

    //load and display an initial image
    queryText = "SELECT * FROM " + this->mTableName.toStdString();
    qry.exec(queryText.c_str());
    QString initFileID;
    qry.first();
    //get the value of last column which is the image path
    // TODO: query for FILE column
    initFileID = qry.value(this->numSliders).toString();

    QString imagePath = database;
    imagePath += "/" + initFileID; //loads the first image from first row in the db
    QPixmap image;

    bool loadSuccess = image.load(imagePath);
    if(!loadSuccess)
    {
        imagePath = database;
        imagePath += "/empty_image/empty.png";
        image.load(imagePath);
    }

    //image.fill(Qt::transparent); // shows a blank screen

    this->mScene->addPixmap(image);
    // this->mImageView = new MyImageView(this->mImagePanel);

    this->mImageView->setScene(this->mScene);
    this->mImageLayout->addWidget(this->mImageView);

    this->mSliderLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    for(int i=0;i<this->numSliders;i++)
    {
        this->mSliderLayout->addRow(this->mListOfSliderLabels[i], this->mListOfSliders[i]);
    }
}

void MainWindow::popSlidersOnValidValue()
{
    string query,query1;
    vector<float> adjustedVals;
    QSqlQuery qry,qry1;
    float val,val1;

    for(int i=0;i<numSliders;i++)
    {
        stringstream ss;
        ss<<this->mListOfSliders[i]->value();

        query = "SELECT min(" + this->mColumnNames[i] + ") FROM " +this->mTableName.toStdString() + " WHERE " + this->mColumnNames[i] + " >= " + ss.str();
        qry.exec(query.c_str());

        query1 = "SELECT max(" + this->mColumnNames[i] + ") FROM " +this->mTableName.toStdString() + " WHERE " + this->mColumnNames[i] + " < " + ss.str();
        qry1.exec(query1.c_str());

        while (qry.next())
        {
            val = qry.value(0).toString().toFloat(); //get the last column which has the image name
        }

        while (qry1.next())
        {
            val1 = qry1.value(0).toString().toFloat(); //get the last column which has the image name
        }

        if(abs(val - this->mListOfSliders[i]->value()) >= abs(val1 - this->mListOfSliders[i]->value()))
            adjustedVals.push_back(val1);
        else
            adjustedVals.push_back(val);
    }

    //Set the values to the sliders on nearest valid values
    for(int i=0;i<numSliders;i++)
    {
        this->mListOfSliders[i]->setValue(adjustedVals[i]);
    }
}

string MainWindow::constructQueryString()
{
    string query;

    query = "SELECT * FROM " +this->mTableName.toStdString() + " WHERE ";
    for(int i=0;i<numSliders;i++)
    {
        if(i<numSliders-1)
            query = query + this->mColumnNames[i] + "=:" + this->mColumnNames[i] + " AND ";
        else
            query = query + this->mColumnNames[i] + "=:" + this->mColumnNames[i];

    }
    return query;
}

void MainWindow::on_slider_valueChanged(int value)
{
    string query = constructQueryString();

    QSqlQuery qry;
    qry.prepare(QString::fromStdString(query));
    for(int i=0;i<numSliders;i++)
    {
        string s;
        s = ":"+this->mColumnNames[i];
        qry.bindValue(QString::fromStdString(s), this->mListOfSliders[i]->value());
    }
    qry.exec();

    while (qry.next())
    {
        QString val = qry.value(this->numSliders).toString(); //get the last column which has the image name

        string path = "../data/volume-render.cdb";

        QString imagePath = QString::fromStdString(path); //this->mCurDatabase; // this->mCurDatabase has NULL
        imagePath += "/";
        imagePath += val;

        //cout<<imagePath.toStdString()<<" "<<this->mCurDatabase.toStdString()<<endl;

        QPixmap image;
        bool loadSuccess = image.load(imagePath);

        if(!loadSuccess)
        {
            cout<<"image loading failed!!"<<endl;
            imagePath = this->mCurDatabase;
            imagePath += "/";
            imagePath += "empty_image/empty.png";
            image.load(imagePath);
        }

        this->mScene->addPixmap(image);
        //this->mImageView->setScene(this->mScene);
    }

    //Pop sliders to a valid value during drag: To overcome Qt's default behavior of sliders that increment step by 1. But our step is not always 1.
    popSlidersOnValidValue();

}

void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    mOpenAction = fileMenu->addAction(tr("&Open ..."), this, &MainWindow::onOpenFile);
    mOpenAction->setShortcut(QKeySequence::Open);
    mOpenAction->setStatusTip(tr("Open a file"));

    fileMenu->addSeparator();

    mQuitAction = fileMenu->addAction(tr("E&xit"), this, &MainWindow::onQuit);
    mQuitAction->setShortcut(tr("Ctrl+Q"));
    mQuitAction->setStatusTip(tr("Quit application"));

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(tr("&About"), this, &MainWindow::onAbout);
}

void MainWindow::onOpenFile()
{
<<<<<<< HEAD
    mCurDatabase = QFileDialog::getExistingDirectory(this,
                                                     tr("Open Cinema Database"), "/",
                                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
=======
    QString result = QFileDialog::getExistingDirectory(this,
        tr("Open Cinema Database"), "/",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
>>>>>>> f5ab17769c78d90dbb660c98c1c52328c3e125c8

    this->loadCinemaDatabase(result);
}

void MainWindow::onQuit()
{
    QApplication::quit();
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, tr("CinemaScope"),
                       tr("This is the about message"));
}

void MainWindow::flushUI()
{
    // flush the database
    QSqlQuery qry;
    qry.exec("DROP TABLE " + this->mTableName);

    // flush the sliders
    int count = this->mSliderLayout->rowCount();
    qDebug() << "COUNT: " << count;
    for (int i=(count-1);i>=0;i--)
    {
        this->mSliderLayout->removeRow(i);
    }

    // flush the vectors of data associated with the sliders
    this->mColumnNames.clear();
    qDebug() << this->mColumnNames.size();
    // note that these objects should already be deleted, per the above call
    this->mListOfSliders.clear();
    qDebug() << this->mListOfSliders.size();
    // note that these objects should already be deleted, per the above call
    this->mListOfSliderLabels.clear();
    qDebug() << this->mListOfSliderLabels.size();
}
