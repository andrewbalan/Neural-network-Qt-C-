#include "layer.h"


Layer::Layer(int numberOfNodes, Layer *previousLayer, QWidget *parent):QWidget(parent), nodesNumber(numberOfNodes), prev(previousLayer)
{

    if(prev) {
        inputLength = prev->nodesNumber + 1;
        weights = new double* [nodesNumber];

        for (int count = 0; count < nodesNumber; count++)
            weights[count] = new double[inputLength];

       // fill with random numbers
        for (int i = 0; i < nodesNumber; ++i) {
            for (int j = 0; j < inputLength; ++j) {
                weights[i][j] = (double)(-500 + qrand() % 1000)/1000;
            }
        }

        error = new double[nodesNumber];
        for (int i = 0; i < nodesNumber; ++i) {
            error[i] = 0;
        }

        // fill derivatives array
        derivative = new double* [nodesNumber];
        for (int count = 0; count < nodesNumber; count++)
            derivative[count] = new double[inputLength];

        for (int i = 0; i < nodesNumber; ++i) {
            for (int j = 0; j < inputLength; ++j) {
                derivative[i][j] = 0;
            }
        }
    } else {
        inputLength = nodesNumber;
        weights = NULL;

        error = NULL;
        derivative = NULL;
        z = NULL;
    }

    values = new double[nodesNumber];
    for (int i = 0; i < nodesNumber; ++i) {
        values[i] = 0;
    }

    z = new double[nodesNumber];
    for (int i = 0; i < nodesNumber; ++i) {
        z[i] = 0;
    }

    input = new double[inputLength];
    for (int i = 0; i < inputLength; ++i) {
        input[i] = 0;
    }


    // UI setup
    ui.setupUi(this);
    uiInitLabels();
    uiInitTables();
}


Layer::~Layer()
{
    if(weights) {
        for (int i = 0; i < nodesNumber; i++) {
            delete [] weights[i];
        }
    }
    if(derivative) {
        for (int i = 0; i < nodesNumber; i++) {
            delete [] derivative[i];
        }
    }
    if(values) {
        delete[] values;
    }
    if(input) {
        delete[] input;
    }
    if(error) {
        delete[] error;
    }
    if(z) {
        delete[] z;
    }
}


void Layer::setInput(double* arr, int arrLength)
{
    if(prev && prev->nodesNumber != arrLength) {
        qFatal("layer.cpp: setInput(): Nodes number does not equal dimension of input vector ");
    }

    if(prev) {
        // add bias unit
        input[0] = 1;
        for (int i = 0; i < (inputLength - 1); ++i) {
            input[i+1] = arr[i];
        }
    } else {
        //first layer
        for (int i = 0; i < inputLength; ++i) {
            input[i] = arr[i];
        }
    }
}

double Layer::getInput(int index)
{
    if(index > inputLength - 1) {
        qFatal("layer.cpp: getInput(): index isn't' equal dimension of the vector ");
    }
    return input[index];
}


void Layer::eval()
{
    if(prev) {
        setInput(prev->values, prev->getNodesNumber());

        double linear_combination;

        for (int i = 0; i < nodesNumber; ++i) {
            linear_combination = 0;
            for (int j = 0; j < inputLength; ++j) {
                linear_combination += weights[i][j] * input[j];
            }
            z[i] = linear_combination;
            values[i] = sigmoid(linear_combination);
        }
    } else {
        for (int i = 0; i < nodesNumber; ++i) {
            values[i] = input[i];
        }
    }
}


void Layer::uiInitLabels()
{
    ui.inputDimensionLabel->setText(QString("%1 * 1").arg(inputLength));
    ui.nodesDimensionLabel->setText(QString("%1 * 1").arg(nodesNumber));

    if(prev) {
        QString tmpStr = QString("%1 * %2").arg(nodesNumber).arg(inputLength);
        ui.weightsDimensionLabel->setText(tmpStr);
    } else {
        ui.weightsDimensionLabel->setText(QString("-"));
    }
}

void Layer::uiInitTables()
{
    ui.inputTable->setRowCount(inputLength);
    ui.weightsTable->setRowCount(nodesNumber);
    ui.weightsTable->setColumnCount(inputLength);
    ui.valuesTable->setRowCount(nodesNumber);

    if(!weights) {
        ui.weightsTable->setFixedWidth(0);
    }

    QTableWidgetItem *ptwi = 0;
    for (int i = 0; i < inputLength; ++i) {
        ptwi = new QTableWidgetItem(QString("%1").arg(input[i]));
        ui.inputTable->setItem(i,0, ptwi);
    }

    if(weights) {
        for (int i = 0; i < nodesNumber; ++i) {
            for (int j = 0; j < inputLength; ++j) {
                QString tmpStr = QString("%1").arg(weights[i][j]);
                ptwi = new QTableWidgetItem(tmpStr);
                ui.weightsTable->setItem(i,j, ptwi);
            }
        }
    }

    for (int i = 0; i < nodesNumber; ++i) {
        ptwi = new QTableWidgetItem(QString("%1").arg(values[i]));
        ui.valuesTable->setItem(i,0, ptwi);
    }
}

void Layer::uiRefreshTables()
{
    QString tmpStr;

    for (int i = 0; i < inputLength; ++i) {
        tmpStr = QString("%1").arg(input[i]);
        ui.inputTable->item(i, 0)->setText(tmpStr);
    }

    if(weights) {
        for (int i = 0; i < nodesNumber; ++i) {
            for (int j = 0; j < inputLength; ++j) {
                QString tmpStr = QString("%1").arg(weights[i][j]);
                ui.weightsTable->item(i, j)->setText(tmpStr);
            }
        }
    }

    for (int i = 0; i < nodesNumber; ++i) {
        tmpStr = QString("%1").arg(values[i]);
        ui.valuesTable->item(i, 0)->setText(tmpStr);
    }
}


int Layer::getNodesNumber() const
{
    return nodesNumber;
}

int Layer::getInputLength() const
{
    return inputLength;
}

double Layer::getLinearCombination(int i) const
{
    if(i > nodesNumber - 1) {
        qFatal("layer.cpp:getLinearCombination(): index is out of array");
    }
    return z[i];
}


double Layer::getWeight(unsigned int row, unsigned int col) const
{
    if(!weights) qFatal("layer.cpp:getWeight(): weights isn't defines");
    if(col > inputLength - 1) {
        qFatal("layer.cpp:getWeight(): col is out of array index");
    }

    if(row > nodesNumber - 1) {
        qFatal("layer.cpp:getWeight(): row is out of array index");
    }

    return weights[row][col];
}


void Layer::setWeight(unsigned int row, unsigned int col, double value)
{
    if(!weights) qFatal("layer.cpp:getWeight(): weights isn't defines");
    if(col > inputLength - 1) {
        qFatal("layer.cpp:setWeight(): col is out of array index");
    }

    if(row > nodesNumber - 1) {
        qFatal("layer.cpp:setWeight(): row is out of array index");
    }

    weights[row][col] = value;
}


double Layer::getDerivative(unsigned int row, unsigned int col) const
{
    if(!weights) qFatal("layer.cpp:getDerivative(): weights isn't defines");
    if(col > inputLength - 1) {
        qFatal("layer.cpp:getDerivative(): col is out of array index");
    }

    if(row > nodesNumber - 1) {
        qFatal("layer.cpp:getDerivative(): row is out of array index");
    }

    return derivative[row][col];
}

void Layer::setDerivative(unsigned int row, unsigned int col, double value)
{
    if(!weights) qFatal("layer.cpp:getWeight(): weights isn't defines");
    if(col > inputLength - 1) {
        qFatal("layer.cpp:setDerivative(): col is out of array index");
    }

    if(row > nodesNumber - 1) {
        qFatal("layer.cpp:setDerivative(): row is out of array index");
    }

    derivative[row][col] = value;
}


const double* Layer::getValues() const
{
    double const *link = this->values;
    return link;
}

double Layer::getValue(int i)
{
    if(i > nodesNumber - 1) {
        qFatal("layer.cpp:getValue(): index is out of array index");
    }
    return values[i];
}


double **Layer::getWeights() const
{
    return this->weights;
}

double Layer::getError(int index)
{
    if(index > nodesNumber - 1) {
        qFatal("layer.cpp:getError(): row is out of array index");
    }
    return error[index];
}

void Layer::setError(int index, double value)
{
    if(index > nodesNumber - 1) {
        qFatal("layer.cpp:setError(): row is out of array index");
    }
    error[index] = value;
}

double Layer::sigmoid(double z)
{
    return 1 / (1 + qExp(-z));
}

double Layer::sigmoidGradient(double z) {
    return sigmoid(z) * ( 1 - sigmoid(z));
}

