#include "perceptron.h"
#include "time.h"

Perceptron::Perceptron(int inputNumber, QWidget *parent) : QWidget(parent)
{
    Layer* p_l = new Layer(inputNumber,NULL,this);
    layer.push_back(p_l);

    layout.addWidget(p_l);
    this->setLayout(&layout);

    lamda = 0.0000001;
    qsrand(time(0));
}

Perceptron::Perceptron(QWidget *parent) : QWidget(parent)
{
    qsrand(time(0));
    lamda = 0.00000000001;
    this->setLayout(&layout);
}

Perceptron::~Perceptron()
{
    foreach (Layer* l, layer) {
        delete l;
    }
}

DataSource *Perceptron::getDataSrc() const
{
    return dataSrc;
}

void Perceptron::setDataSrc(DataSource *value)
{
    dataSrc = value;
}

void Perceptron::slot_cost()
{
    qDebug() << "cost:" << cost();
}

void Perceptron::addLayer(int inputNumber)
{
    Layer* prev = NULL;
    if(!layer.isEmpty()) prev = layer.last();
    Layer* p_l = new Layer(inputNumber,prev);
    layer.push_back(p_l);
    layout.addWidget(p_l);
}


double Perceptron::cost()
{
    if(!dataSrc) {
        qFatal("perceptron.cpp: cost() : Data source wasn't defined");
    }

    int m = getTrainigSetNumber();
    int k = getClassesAmount();
    int L = layer.count();
    int inputVectorDimension = dataSrc->getVectorDimension();
    double summ = 0;
    const double* h;
    double* x;

    if(k != dataSrc->getClassesAmount()) {
        qFatal("perceptron.cpp: cost() : Number of classes isn't equal type of data");
    }

    for (int i = 0; i < m; ++i) {
        //get x
        x = dataSrc->getData(i);
        //get y
        int* y = dataSrc->getLabelAsArray(i);

        //get h
        setInput(x, inputVectorDimension);
        h = getOutput();

        for (int j = 0; j < k; ++j) {
            double H = h[j];
            if(h[j] == 0) H = 0.00000001;
            summ += y[j] * qLn(H) + (1 - y[j]) * qLn(1 - H);
        }

        // calc progress
        float perc =((float)100/m) * i;
        emit statCost(qCeil(perc));
    }
    summ = (-1 / (double)m) * summ;

    // regularization
    double reg_summ = 0;

    for (int l = 1; l < layer.length(); ++l) {
        Layer* pt_l = layer.at(l);
        for (int i = 0; i < pt_l->getNodesNumber(); ++i) {
            for (int j = 0; j < pt_l->getInputLength(); ++j) {
                reg_summ += qPow( pt_l->getWeight(i,j), 2);
            }
        }
    }

    reg_summ *= (double) lamda / 2*m;
    return summ + reg_summ;
}


void Perceptron::gradientDescent()
{
    for (int it = 0; it < 50; ++it) {
        resetDerivatives();
        int length = layer.length();
        Layer* current;
        int m = dataSrc->getTrainingExampleAmount();

        for (int i = 0; i < m; ++i) {
            double* example = dataSrc->getData(i);
            int exampleLength = dataSrc->getVectorDimension();

            setInput(example, exampleLength);
            getOutput();
            calculateDelta(i);

            // derivatives computation
            for (int l = 1; l < length; ++l) {
                current = layer.at(l);
                for (int r = 0; r < current->getNodesNumber(); ++r) {
                    for (int c = 0; c < current->getInputLength(); ++c) {
                        Layer* prev = layer.at(l - 1);
                        double delta = current->getError(r);
                        double a = current->getInput(c); // add 1 to a
                        double D = current->getDerivative(r,c) + delta * a;
                        current->setDerivative(r,c, D);
                    }
                }
            }

            // calc progress
            float perc =((float)100/m) * i;
            emit statCost(qCeil(perc));
        }

        // derivatives regularization
        for (int l = 1; l < length; ++l) {
            current = layer.at(l);
            for (int r = 0; r < current->getNodesNumber(); ++r) {
                for (int c = 0; c < current->getInputLength(); ++c) {
                    if(r != 0) {
                        double D = current->getDerivative(r,c) + lamda * current->getWeight(r,c);
                        current->setDerivative(r,c, D);
                    }
                    double dt = ((double) 1/m) * current->getDerivative(r,c);
                    //qDebug() << dt;
                    current->setDerivative(r,c, dt);

                    double w = current->getWeight(r,c) - current->getDerivative(r,c);
                    current->setWeight(r,c,w);
                }
            }
        }

        uiRefresh();
        qDebug() << "Cost: " << cost();
    }
}


void Perceptron::calculateDelta(int index)
{
    // DELTA'S CALCULATION
    Layer* current;
    Layer* next;
    for (int l = layer.length() - 1; l > 0; --l) {
        current = layer.at(l);
        int nodeAmount = current->getNodesNumber();

        if(l != layer.length() - 1) {
            // NON-LAST LAYER CALCULATION
            next = layer.at(l + 1);

            for (int i = 0; i < current->getNodesNumber()+1; ++i) {
                double lin_comb = 0;
                for (int j = 0; j < next->getNodesNumber(); ++j) {
                    lin_comb += next->getWeight(j, i) * next->getError(j);
                }
                double grad ;//= Layer::sigmoidGradient(current->getValue(i-1));
                if(i == 0) {
                    grad = Layer::sigmoidGradient(1);
                } else {
                    grad = Layer::sigmoidGradient(current->getValue(i-1));
                }
                if(i != 0) current->setError(i-1 , lin_comb * grad);
            }

        } else {
            for (int j = 0; j < nodeAmount; ++j) {
                double a = current->getValues()[j];
                double y = (double) (dataSrc->getLabelAsArray(index)[j]);
                double delta = a - y;
                current->setError(j, delta);
            }
        }
    }
}

void Perceptron::resetDerivatives()
{
    Layer *l;
    for (int var = 1; var < layer.length(); ++var) {
        l = layer.at(var);
        for (int i = 0; i < l->getNodesNumber(); ++i) {
            for (int j = 0; j < l->getInputLength(); ++j) {
                l->setDerivative(i,j,0);
            }
        }
    }
}

int Perceptron::getAnswer()
{
    int classes = dataSrc->getClassesAmount();
    double max = layer.last()->getValue(0);
    int index = 0;

    for (int i = 1; i < classes; ++i) {
        if (layer.last()->getValue(i) > max ) {
            max = layer.last()->getValue(i);
            index = i;
        }
    }
    return index;
}

void Perceptron::setInput(double* arr, int length)
{
    if(layer.isEmpty()) {
        qFatal("perceptron.cpp: setInput : First layer isn't define");
        return;
    }

    if(layer[0]->getNodesNumber() != length) {
        qDebug() << "nodes number : " << layer[0]->getNodesNumber();
        qDebug() << "array length : " << length;
        qFatal("perceptron.cpp: setInput : Input doesn't equal nodes number");
        return;
    }

    layer[0]->setInput(arr, length);
}

const double* Perceptron::getOutput()
{
    Layer* p_layer = NULL;
    foreach (p_layer, layer) {
        p_layer->eval();
    }
    return layer.last()->getValues();
}

int Perceptron::getTrainigSetNumber()
{
    if(dataSrc) {
        return dataSrc->getTrainingExampleAmount();
    } else {
        qDebug("perceptron.cpp:getTrainigSetNumber: dataSrc doesn't define");
        return 0;
    }
}

int Perceptron::getClassesAmount()
{
    if(!layer.isEmpty()) {
        return layer.last()->getNodesNumber();
    } else {
        qFatal("Perceptron:getclassesAmount: container is empty");
    }
}

// public slots

void Perceptron::removeLastLayer()
{
    if(!layer.isEmpty()) {
        Layer* p_l = layer.last();
        layout.removeWidget(p_l);
        delete p_l;
        layer.removeLast();
    }
}

void Perceptron::uiRefresh()
{
    Layer* p_l;
    foreach (p_l, layer) {
        p_l->uiRefreshTables();
    }
}

void Perceptron::saveAll()
{

    QFile file("file.dat");

    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);   // we will serialize the data into the file

        out << (qint32)layer.size(); // number of layers
        Layer* instance;
        foreach (instance, layer) {
            int rows = instance->getNodesNumber(); // rows
            int cols = instance->getInputLength(); // cols

            out << (qint32) rows;
            out << (qint32) cols;

            if(instance->getWeights()) {
                out << true;
                for (int i = 0; i < rows; ++i) {
                    for (int j = 0; j < cols; ++j) {
                        out << instance->getWeight(i,j);
                    }
                }
            } else {
                out << false;
            }
        }

        if(out.status() != QDataStream::Ok) {
            qDebug() << "perceptron.cpp:saveAll: save error";
        }
    }

    file.close();
}

void Perceptron::loadAll()
{
    QFile file("file.dat");
    if (file.open(QIODevice::ReadOnly)) {
        while(!layer.isEmpty()) {
            removeLastLayer();
        }

        QDataStream in(&file);    // read the data serialized from the file
        qint32 layersAmount;
        qint32 nodesNumber;
        qint32 inputLength;
        bool   flag;

        in >> layersAmount;
        qDebug() << "Layers amount:" << layersAmount;
        while(!in.atEnd()) {
            in >> nodesNumber >> inputLength>>flag;
            qDebug() << "-------------------------------";
            qDebug() << "Inputs amount :" << inputLength;
            qDebug() << "Neurons amount:" << nodesNumber;

            if(flag) {
                addLayer(nodesNumber);
                for (int i = 0; i < nodesNumber; ++i) {
                    for (int j = 0; j < inputLength; ++j) {
                        double tmp;
                        in >> tmp;
                        layer.last()->setWeight(i, j, tmp);
                    }
                }
            } else {
                addLayer(nodesNumber);
            }
            layer.last()->uiRefreshTables();
        }
    }
    file.close();
}
