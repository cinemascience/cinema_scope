#include "mainwindow.h"
#include <QSizePolicy>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>

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
    cout<<"moving mouse loc: "<< p.rx()<<" "<<p.ry()<<endl;
}

/*void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QRect rect = e->rect();
    painter.drawImage(rect,image,rect);
}*/

/*class MyWidget : public QWidget
{
public:
    QImage image = QImage("/home/soumya/Shared_Vbox/cinema_project_codes/cinema_scope/data/volume-render.cdb/images/1.jpg");

    void paintEvent(QPaintEvent *e)
    {
        QWidget::paintEvent(e);
        QPainter painter(this);
        QRect rect = e->rect();
        painter.drawImage(rect,image,rect);
    }
};*/

MainWindow::MainWindow(QSqlDatabase db, QString path, QWidget *parent) : QMainWindow(parent)
{
    QWidget *mainWindow = new QWidget;
    mainWindow->setWindowTitle("Test window");
    mainWindow->resize(600,600);
    setCentralWidget(mainWindow);
    createActions();
    setUnifiedTitleAndToolBarOnMac(true);
    menuBar()->setNativeMenuBar(false);


    ///////////////////////////////////////////////////////////
    /// extract information from sql database

    QStringList tablesList = db.tables(); //get all the tables. We should have just one
    tname = tablesList[0]; // get the table name
    cout<<"Name of the table: "<<tname.toStdString()<<endl;
    //cout<<tablesList.length()<<endl; // should be 1 for now since we handle one cinemaBD

    QSqlQuery qry;
    string queryText = "SELECT * FROM " + tname.toStdString();
    qry.exec(queryText.c_str());

    //Get number of sliders = number of columns-1
    this->numSliders = qry.record().count()-1;
    cout<<"Number of columns: "<<this->numSliders+1<<endl;

    //Get column names
    for(int i=0;i<qry.record().count();i++)
    {
        columnNames.push_back(db.driver()->record(tname).fieldName(i).toStdString());
    }

    //Get value ranges of each column
    vector<float> minVals;
    vector<float> maxVals;
    for(int i=0;i<this->numSliders;i++) //since last column is image file names
    {
        queryText = "SELECT MIN(" + columnNames[i] + ") , MAX(" + columnNames[i] + ") FROM " + tname.toStdString();
        qry.exec(queryText.c_str());
        while (qry.next())
        {
            minVals.push_back(qry.value(0).toFloat());
            maxVals.push_back(qry.value(1).toFloat());
            cout<<"Range of column "<<columnNames[i]<<" is: "<<minVals[i]<<" "<<maxVals[i]<<endl;
        }
    }

    // slider section
    listOfSliders.resize(this->numSliders);
    listOfSliderLabels.resize(this->numSliders);
    for(int i=0;i<this->numSliders;i++)
    {
        listOfSliders[i] = new QSlider(Qt::Horizontal);
        listOfSliders[i]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
        listOfSliders[i]->setRange(minVals[i],maxVals[i]);
        //listOfSliders[i]->setSingleStep(11);
        QObject::connect(listOfSliders[i],SIGNAL(valueChanged(int)),this,SLOT(on_slider_valueChanged(int)));
        stringstream ss;
        ss<<i;
        string label = columnNames[i];
        listOfSliderLabels[i] = new QLabel;
        listOfSliderLabels[i]->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        listOfSliderLabels[i]->setText(label.c_str());
    }

    //load and display an initial image
    rootPath = path.toStdString();
    queryText = "SELECT * FROM " + tname.toStdString();
    qry.exec(queryText.c_str());
    string initFileID;
    qry.first();
    initFileID = qry.value(this->numSliders).toString().toStdString(); //get the value of last column which is the image path

    string imagePath = rootPath + "/" + initFileID; //loads the first image from first row in the db
    QPixmap image;

    bool loadSuccess = image.load(imagePath.c_str());
    if(!loadSuccess)
    {
        imagePath = rootPath + "/" + "empty_image/empty.png";
        image.load(imagePath.c_str());
    }

    //image.fill(Qt::transparent); // shows a blank screen

    /*QWidget* wrapper = new QWidget();
    MyWidget* overlay = new MyWidget();
    overlay->setParent(wrapper);
    wrapper->show();*/

    scene = new QGraphicsScene;
    scene->addPixmap(image);
    //scene->addWidget(wrapper);
    imageView = new QGraphicsView;
    imageView->setScene(this->scene);

    QHBoxLayout *layout1 = new QHBoxLayout;
    layout1->addWidget(imageView);

    QFormLayout *layout2 = new QFormLayout;
    layout2->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    for(int i=0;i<this->numSliders;i++)
    {
        layout2->addRow(listOfSliderLabels[i], listOfSliders[i]);
    }

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(layout1);
    mainLayout->addLayout(layout2);

    mainWindow->setLayout(mainLayout);

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
        ss<<listOfSliders[i]->value();

        query = "SELECT min(" + columnNames[i] + ") FROM " + tname.toStdString() + " WHERE " + columnNames[i] + " >= " + ss.str();
        qry.exec(query.c_str());

        query1 = "SELECT max(" + columnNames[i] + ") FROM " + tname.toStdString() + " WHERE " + columnNames[i] + " < " + ss.str();
        qry1.exec(query1.c_str());

        while (qry.next())
        {
            val = qry.value(0).toString().toFloat(); //get the last column which has the image name
        }

        while (qry1.next())
        {
            val1 = qry1.value(0).toString().toFloat(); //get the last column which has the image name
        }

        if(abs(val-listOfSliders[i]->value()) >= abs(val1-listOfSliders[i]->value()))
            adjustedVals.push_back(val1);
        else
            adjustedVals.push_back(val);
    }

    //Set the values to the sliders on nearest valid values
    for(int i=0;i<numSliders;i++)
    {
        listOfSliders[i]->setValue(adjustedVals[i]);
    }
}

string MainWindow::constructQueryString()
{
    string query;

    query = "SELECT * FROM " + tname.toStdString() + " WHERE ";
    for(int i=0;i<numSliders;i++)
    {
        if(i<numSliders-1)
            query = query + columnNames[i] + "=:" + columnNames[i] + " AND ";
        else
            query = query + columnNames[i] + "=:" + columnNames[i];

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
        s = ":"+columnNames[i];
        qry.bindValue(QString::fromStdString(s), listOfSliders[i]->value());
    }
    qry.exec();

    while (qry.next())
    {
        string val = qry.value(this->numSliders).toString().toStdString(); //get the last column which has the image name
        stringstream ss;
        ss << val;
        string imagePath = rootPath + "/" + ss.str();
        QPixmap image;
        bool loadSuccess = image.load(imagePath.c_str());
        if(!loadSuccess)
        {
            imagePath = rootPath + "/" + "empty_image/empty.png";
            image.load(imagePath.c_str());
        }

        scene->addPixmap(image);
        imageView->setScene(this->scene);
    }

    //Pop sliders to a valid value during drag: To overcome Qts default behavior of sliders that increment step by 1. But our step is not always 1.
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
    mCurDatabase = QFileDialog::getExistingDirectory(this,
        tr("Open Cinema Database"), "/",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    // TODO: check the database
    qDebug() << mCurDatabase;
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
