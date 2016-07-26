#include "mainview.h"
#include "ui_mainview.h"

MainView::MainView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainView)
{

    dataSrc = new DataSource(784,10, this);

    // Setup files with data
    dataSrc->setDataFilename("train-images.dat");
    dataSrc->setLabelFilename("train-labels.dat");

    ui->setupUi(this);
    ui->readMNIST->setDataSrc(dataSrc);
    ui->readMNIST->readData(0);

    ui->perceptron->addLayer(784);
    ui->perceptron->addLayer(298);
    ui->perceptron->addLayer(64);
    ui->perceptron->addLayer(10);

    ui->perceptron->setDataSrc(dataSrc);

    QObject::connect(ui->saveBtn, SIGNAL(clicked()),
                     ui->perceptron, SLOT(saveAll())
                     );
    QObject::connect(ui->loadBtn, SIGNAL(clicked()),
                     ui->perceptron, SLOT(loadAll())
                     );
    QObject::connect(ui->rmBtn, SIGNAL(clicked()),
                     ui->perceptron, SLOT(removeLastLayer())
                     );
    QObject::connect(ui->costBtn, SIGNAL(clicked()),
                     ui->perceptron, SLOT(slot_cost())
                     );
    QObject::connect(ui->perceptron, SIGNAL(statCost(int)),
                     ui->prBar1, SLOT(setValue(int))
                     );
    QObject::connect(ui->descentBtn, SIGNAL(clicked()),
                     ui->perceptron, SLOT(gradientDescent())
                     );
    QObject::connect(ui->inputBtn, SIGNAL(clicked()),
                      SLOT(setInput())
                     );
    QObject::connect(ui->perceptron, SIGNAL(calculateForExample(int)),
                     ui->readMNIST, SLOT(setCurrentNum(int))
                     );
    QObject::connect(ui->accuracyBtn, SIGNAL(clicked()),
                     SLOT(checkAccuracy())
                     );
}



MainView::~MainView()
{
    delete dataSrc;
    delete ui;
}

void MainView::setInput()
{
    int num = ui->readMNIST->getCurrentNum();
    double* vec = dataSrc->getData(num);
    int length = dataSrc->getVectorDimension();
    int classesAmount = ui->perceptron->getClassesAmount();
    ui->perceptron->setInput(vec, length);
    const double* ptr = ui->perceptron->getOutput();
    ui->perceptron->uiRefresh();


    QMap<double, int> arr;

    for (int i = 0; i < classesAmount; ++i) {
        arr.insert(ptr[i],i);
    }

    QString str = "Prediction:\n";

    int count = 3;
    QMapIterator<double, int> i(arr);
    i.toBack();
    while (count) {
        i.previous();
        str += QString::number(i.value()) +" - " + QString::number(i.key());
        str += "\n";
        count--;
    }


    ui->predictionLbl->setText(str);
}

void MainView::checkAccuracy()
{
    int totalExamples = dataSrc->getTrainingExampleAmount();
    int rightAnswers = 0;
    for (int i = 0; i < totalExamples; ++i) {
        ui->perceptron->setInput(dataSrc->getData(i), dataSrc->getVectorDimension());
        ui->perceptron->getOutput();
        int answer = ui->perceptron->getAnswer();
        int label = (int)dataSrc->getLabel(i);

        if(label == answer) {
            ++rightAnswers;
        }
        float perc =((float)100/totalExamples) * i;
        ui->prBar1->setValue(perc);
    }

    QString str = "Accuracy : " + QString::number(((double)rightAnswers / dataSrc->getTrainingExampleAmount()) * 100) + "%" ;
    str.append("| Right answers: " + QString::number(rightAnswers) + " / " + QString::number(totalExamples)) ;
    ui->accuracyLbl->setText(str);
}
