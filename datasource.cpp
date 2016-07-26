#include "datasource.h"
#include <QDebug>



DataSource::DataSource(int outputDimension, int classes, QObject *parent) : QObject(parent)
{
    dataFile = new QFile();
    labelFile = new QFile();
    vectorDimension = outputDimension;
    trainingExamplesAmount = 2000;
    classesAmount = classes;

    label_arr = new int[classesAmount];
    for (int i = 0; i < classesAmount; ++i) {
        label_arr[i] = 0;
    }

    data = new double[vectorDimension];
    for (int i = 0; i < vectorDimension; ++i) {
        data[i] = 0;
    }
}

DataSource::~DataSource()
{
    delete dataFile;
    delete labelFile;
    delete[] data;
    delete[] label_arr;
}

bool DataSource::setDataFilename(QString filename)
{
    dataFile->setFileName(filename);
    if(!dataFile->exists()) {
        qDebug() << "datasource.cpp:setDataFilename(): Data file does not exists";
        return false;
    }
    return true;
}

bool DataSource::setLabelFilename(QString filename)
{
    labelFile->setFileName(filename);
    if(!labelFile->exists()) {
        qDebug() << "datasource.cpp:setLabelFilename(): Label file does not exists";
        return false;
    }
    return true;
}

double* DataSource::getData(int param) const
{
    if(param > trainingExamplesAmount) return NULL;
    if(!dataFile->open(QIODevice::ReadOnly)) {
        qDebug() << "Can't open file";
        return NULL;
    }
    char bytes[vectorDimension];
    // 4 * 4 - 32bit integer
    dataFile->read(16 + param * sizeof(bytes));
    dataFile->read(bytes, sizeof(bytes));
    dataFile->close();

    for (int i = 0; i < vectorDimension; ++i) {
        data[i] = (unsigned char)bytes[i];
    }

    return data;
}

double DataSource::getLabel(int param)
{
    if(param > trainingExamplesAmount)
        qFatal("datasource.cpp:getLabel(): param is out of trainingExamplesAmount");
    if(!labelFile->open(QIODevice::ReadOnly)) {
        qDebug() << "Can't open file";
        return 0;
    }
    //4 * 2 - 32bit integer
    labelFile->read(8 + param * 1);
    char a = 255;
    labelFile->read(&a, 1);
    label = (double)a;

    labelFile->close();
    return label;
}

int* DataSource::getLabelAsArray(int y)
{
    double val = getLabel(y);

    for (int i = 0; i < classesAmount; ++i) {
        if(i != val) {
            label_arr[i] = 0;
        } else {
            label_arr[i] = 1;
        }
    }
    return label_arr;
}

int DataSource::getTrainingExampleAmount() const
{
    return trainingExamplesAmount;
}


int DataSource::getVectorDimension() const
{
    return vectorDimension;
}

int DataSource::getClassesAmount() const
{
    return classesAmount;
}
