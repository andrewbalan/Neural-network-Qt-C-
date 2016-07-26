#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include <QWidget>
#include <QScrollArea>
#include <QDebug>
#include <QtMath>
#include "layer.h"
#include "datasource.h"

class Perceptron : public QWidget
{
    Q_OBJECT

    QVBoxLayout layout;
    DataSource* dataSrc;
    double lamda;

public:
    QVector<Layer*> layer;

    explicit Perceptron(int inputNumber,QWidget *parent = 0);
    explicit Perceptron(QWidget *parent = 0);
    ~Perceptron();

    void addLayer(int inputNumber);


    const double *getOutput();
    DataSource *getDataSrc() const;
    void setDataSrc(DataSource *value);
    int getTrainigSetNumber();
    int getClassesAmount();

    void calculateDelta(int index);
    void setInput(double* , int);
    double cost();
    void resetDerivatives();
    int getAnswer();


signals:
    void statCost(int);
    void calculateForExample(int);

public slots:
    void gradientDescent();
    void slot_cost();
    void saveAll();
    void loadAll();
    void removeLastLayer();
    void uiRefresh();
protected:
};

#endif // PERCEPTRON_H
