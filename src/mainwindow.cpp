#include "mainwindow.h"
#include <QFormLayout>
#include "CinDBReader.h"

MainWindow::MainWindow(int numSliders, string path, QWidget *parent) : QMainWindow(parent)
{
    QWidget *mainWindow = new QWidget;
    mainWindow->setWindowTitle("Test window");
    mainWindow->resize(600,600);

    //load and display image
    rootPath = path;
    string imagePath = rootPath + "1.jpg"; //loads the first image for now
    QPixmap image;
    image.load(imagePath.c_str());

    scene = new QGraphicsScene;
    scene->addPixmap(image);
    imageView = new QGraphicsView;
    imageView->setScene(this->scene);

    // slider section
    this->numSliders = numSliders;
    listOfSliders.resize(this->numSliders);
    listOfSliderLabels.resize(this->numSliders);
    for(int i=0;i<this->numSliders;i++)
    {
        listOfSliders[i] = new QSlider(Qt::Horizontal);
        listOfSliders[i]->setRange(1,1296);
        QObject::connect(listOfSliders[i],SIGNAL(valueChanged(int)),this,SLOT(on_slider_valueChanged(int)));
        stringstream ss;
        ss<<i;
        string label = "slider_" + ss.str();
        listOfSliderLabels[i] = new QLabel;
        listOfSliderLabels[i]->setText(label.c_str());

    }

    // testing
    CinDBReader reader;
    reader.readCSV("/Users/dhr/LANL/git/github/cinemascience/cinema_scope/data/volume-render/data.csv");

    QHBoxLayout *layout1 = new QHBoxLayout;
    layout1->addWidget(imageView);

    QFormLayout *layout2 = new QFormLayout;
    for(int i=0;i<this->numSliders;i++)
    {
        layout2->addRow(listOfSliderLabels[i], listOfSliders[i]); 
    }

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(layout1);
    //mainLayout->addStretch(10);
    mainLayout->addLayout(layout2);

    mainWindow->setLayout(mainLayout);
    setCentralWidget(mainWindow);

}

MainWindow::~MainWindow()
{

}

void MainWindow::on_slider_valueChanged(int value)
{

//    for(int i=0;i<this->numSliders;i++)
//    {
//        cout<<"value of slider "<<i<<" is: "<<listOfSliders[i]->value()<<endl;
//    }
//    cout<<endl<<endl;

    //QString s = QDir::currentPath();
    //cout<<s.toStdString()<<endl;

    stringstream ss;
    ss << value; //listOfSliders[0]->value();
    string imagePath = rootPath + ss.str() + ".jpg";
    QPixmap image;
    image.load(imagePath.c_str());

    scene->addPixmap(image);
    imageView->setScene(this->scene);
}
