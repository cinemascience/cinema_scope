#include "mainwindow.h"


MainWindow::~MainWindow()
{

}

MainWindow::MainWindow(QSqlDatabase db, string path, QWidget *parent) : QMainWindow(parent)
{
    QWidget *mainWindow = new QWidget;
    mainWindow->setWindowTitle("Test window");
    mainWindow->resize(600,600);

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
        listOfSliders[i]->setRange(minVals[i],maxVals[i]);
        listOfSliders[i]->setSingleStep(11);
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

string MainWindow::constructQueryString(vector<float> currentVals)
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

    vector<float> currentSLiderVals;
    for(int i=0;i<this->numSliders;i++)
    {
        currentSLiderVals.push_back(listOfSliders[i]->value());
    }

    for(int i=0;i<this->numSliders;i++)
    {
        cout<<listOfSliders[i]->value()<<" ";
    }
    cout<<endl;

    string query = constructQueryString(currentSLiderVals);

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
        //cout<<val<<endl;
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
}
