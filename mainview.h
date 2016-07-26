#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QWidget>
#include "perceptron.h"
#include "datasource.h"

namespace Ui {
class MainView;
}

class MainView : public QWidget
{
    Q_OBJECT

    DataSource* dataSrc;
public:
    explicit MainView(QWidget *parent = 0);
    ~MainView();

private:
    Ui::MainView *ui;

public slots:
    void setInput();
    void checkAccuracy();
    //void eval();
};

#endif // MAINVIEW_H
