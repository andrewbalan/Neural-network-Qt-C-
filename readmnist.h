#ifndef READMNIST_H
#define READMNIST_H

#include <QWidget>
#include <QDebug>
#include <QFile>
#include "datasource.h"

namespace Ui {
class ReadMNIST;
}

class ReadMNIST : public QWidget
{
    Q_OBJECT
    unsigned char label;
    DataSource* p_dataSource;


public slots:
    void readData(int param);
    void setCurrentNum(int param);

public:
    explicit ReadMNIST(QWidget *parent = 0);
    ~ReadMNIST();

    void setDataSrc(DataSource *value);
    int getCurrentNum() const;

private:
    Ui::ReadMNIST *ui;
};

#endif // READMNIST_H
