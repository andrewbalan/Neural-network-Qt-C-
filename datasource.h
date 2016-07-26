#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QFile>

class DataSource : public QObject
{
    Q_OBJECT

    QString databaseFilename;
    QString labelsFilename;
    int vectorDimension;
    int trainingExamplesAmount;
    int classesAmount;
    QFile* dataFile;
    QFile* labelFile;
    double* data;
    int* label_arr;
    double label;

public:
    explicit DataSource(int outputDimension,int classes, QObject *parent = 0);
    ~DataSource();

    bool setDataFilename(QString filename);
    bool setLabelFilename(QString filename);
    double* getData(int) const;
    double getLabel(int);
    int* getLabelAsArray(int param);

    int getTrainingExampleAmount() const;
    int getVectorDimension() const;
    int getClassesAmount() const;
signals:

public slots:
};

#endif // DATASOURCE_H
