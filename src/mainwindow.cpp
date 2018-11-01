#include "mainwindow.h"

MainWindow::MainWindow(string path, QWidget *parent) : QMainWindow(parent)
{
    QWidget *mainWindow = new QWidget;
    mainWindow->setWindowTitle("Test window");
    mainWindow->resize(600,600);

    //load and display image
    rootPath = path;
    string imagePath = rootPath + "images/1.jpg"; //loads the first image for now
    QPixmap image;
    image.load(imagePath.c_str());

    scene = new QGraphicsScene;
    scene->addPixmap(image);
    imageView = new QGraphicsView;
    imageView->setScene(this->scene);

    // slider section

    QSqlQuery qry;
    qry.exec("SELECT * FROM dummyCinemaDB");

    //number of sliders = number of columns -1
    this->numSliders = qry.record().count()-1;

    listOfSliders.resize(this->numSliders);
    listOfSliderLabels.resize(this->numSliders);
    for(int i=0;i<this->numSliders;i++)
    {
        listOfSliders[i] = new QSlider(Qt::Horizontal);
        listOfSliders[i]->setRange(1,99); //hard coded for now
        QObject::connect(listOfSliders[i],SIGNAL(valueChanged(int)),this,SLOT(on_slider_valueChanged(int)));
        stringstream ss;
        ss<<i;
        string label = "slider_" + ss.str();
        listOfSliderLabels[i] = new QLabel;
        listOfSliderLabels[i]->setText(label.c_str());
    }

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
    string val = qry.value(2).toString().toStdString(); //get the last column which has the image name

    stringstream ss;
    ss << val;
    string imagePath = rootPath + ss.str();
    QPixmap image;
    image.load(imagePath.c_str());

    scene->addPixmap(image);
    imageView->setScene(this->scene);
}
