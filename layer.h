#ifndef LAYER_H
#define LAYER_H

#include "ui_layerview.h"
#include <QWidget>
#include <QDebug>
#include <QtMath>

class Layer: public QWidget
{
    Q_OBJECT
private:
    const Layer* prev;
    const int nodesNumber;
    int inputLength;
    double** weights;
    double* values;
    double* input;
    double* z; // linear combination

    double* error;
    double **derivative;

    Ui::layerView ui;

public:
    explicit Layer(int numberOfNodes, Layer* previousLayer, QWidget *parent = 0);
    ~Layer();
    void setInput(double* , int);
    double getInput(int index);

    void eval();
    double const * getValues() const;
    double **getWeights() const;
    int getNodesNumber() const;
    int getInputLength() const;
    double getLinearCombination(int i) const;

    double getWeight(unsigned int row, unsigned int col) const;
    void setWeight(unsigned int row, unsigned int col, double value);

    double getDerivative(unsigned int row, unsigned int col) const;
    void setDerivative(unsigned int row, unsigned int col, double value);

    void uiRefreshTables();
    void uiInitLabels();
    void uiInitTables();

    double getError(int index);
    void setError(int index, double value);

    static double sigmoid(double);
    static double sigmoidGradient(double z);
    double getValue(int i);
};

#endif // LAYER_H
