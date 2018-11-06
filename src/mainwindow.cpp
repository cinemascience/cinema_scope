#include "mainwindow.h"
#include "CinDBReader.h"

MainWindow::MainWindow(QSqlDatabase db, string path, QWidget *parent) : QMainWindow(parent)
{
    QWidget *mainWindow = new QWidget;
    mainWindow->setWindowTitle("Test window");
    mainWindow->resize(600,600);

    ///////////////////////////////////////////////////////////
    /// extract information from sql database

    QStringList tablesList = db.tables(); //get all the tables. We should have just one
    QString tname = tablesList[0]; // get the table name
    cout<<"Name of the table: "<<tname.toStdString()<<endl;
    //cout<<tablesList.length()<<endl; // should be 1 for now since we handle one cinemaBD

    QSqlQuery qry;
    string queryText = "SELECT * FROM " + tname.toStdString();
    qry.exec(queryText.c_str());

    //Get number of sliders = number of columns-1
    this->numSliders = qry.record().count()-1;

    //Get column names
    vector<string> columnNames;
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
        listOfSliders[i]->setRange(minVals[i],maxVals[i]); //hard coded for now
        QObject::connect(listOfSliders[i],SIGNAL(valueChanged(int)),this,SLOT(on_slider_valueChanged(int)));
        stringstream ss;
        ss<<i;
        string label = columnNames[i];
        listOfSliderLabels[i] = new QLabel;
        listOfSliderLabels[i]->setText(label.c_str());
    }

    //load and display an initial image
    rootPath = path;
    queryText = "SELECT * FROM " + tname.toStdString();
    qry.exec(queryText.c_str());
    string initFileID;
    qry.first();
    initFileID = qry.value(this->numSliders).toString().toStdString(); //get the value of last column which is the image path

    string imagePath = rootPath + initFileID; //loads the first image from first row in the db
    //cout<<imagePath<<endl;
    QPixmap image;
    bool loadSuccess = image.load(imagePath.c_str());
    if(!loadSuccess)
    {
        imagePath = rootPath + "empty_image/empty.png";
        image.load(imagePath.c_str());
    }

    scene = new QGraphicsScene;
    scene->addPixmap(image);
    imageView = new QGraphicsView;
    imageView->setScene(this->scene);

    // testing
    CinDBReader reader;
    reader.readCSV(db, "/Users/dhr/LANL/git/github/cinemascience/cinema_scope/data/volume-render/data.csv");

    QHBoxLayout *layout1 = new QHBoxLayout;
    layout1->addWidget(imageView);

    QFormLayout *layout2 = new QFormLayout;
    for(int i=0;i<this->numSliders;i++)
    {
        layout2->addRow(listOfSliderLabels[i], listOfSliders[i]);
    }

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(layout1);
    mainLayout->addLayout(layout2);

    mainWindow->setLayout(mainLayout);
    setCentralWidget(mainWindow);
}

MainWindow::~MainWindow()
{

}

void MainWindow::on_slider_valueChanged(int value)
{
    QSqlQuery qry;
    qry.prepare("SELECT * FROM dummyCinemaDB WHERE theta=:theta AND phi=:phi");
    qry.bindValue(":theta", listOfSliders[0]->value());
    qry.bindValue(":phi", listOfSliders[1]->value());
    qry.exec();

    qry.first(); // to get the first record.
    string val = qry.value(this->numSliders).toString().toStdString(); //get the last column which has the image name

    stringstream ss;
    ss << val;
    string imagePath = rootPath + ss.str();
    QPixmap image;
    bool loadSuccess = image.load(imagePath.c_str());
    if(!loadSuccess)
    {
        imagePath = rootPath + "empty_image/empty.png";
        image.load(imagePath.c_str());
    }

    scene->addPixmap(image);
    imageView->setScene(this->scene);
}
