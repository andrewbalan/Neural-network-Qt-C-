#include "mainview.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QScrollArea>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainView wgt(NULL);

    wgt.show();

    return a.exec();
}
