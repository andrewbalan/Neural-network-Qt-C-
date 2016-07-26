#include "readmnist.h"
#include "ui_readmnist.h"


ReadMNIST::ReadMNIST(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReadMNIST)
{
    label = 0;
    ui->setupUi(this);
}

ReadMNIST::~ReadMNIST()
{
    delete ui;
}


void ReadMNIST::setDataSrc(DataSource *value)
{
    p_dataSource = value;
}

int ReadMNIST::getCurrentNum() const
{
    return ui->spinBox->value();
}


void ReadMNIST::readData(int param)
{
    ui->digitViewer->changeData(p_dataSource->getData(param));
    label = (unsigned char) p_dataSource->getLabel(param);
    QString str = "Count: " + QString::number(param) + " | Digit: " + QString::number(label);
    ui->statusLbl->setText(str);
}

void ReadMNIST::setCurrentNum(int param)
{
    if(param > p_dataSource->getTrainingExampleAmount()) {
        qFatal("ReadMNIST:setCurrentNum(): param out of index");
    }
    ui->spinBox->setValue(param);
}
