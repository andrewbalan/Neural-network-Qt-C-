#ifndef DIGITVIEWER_H
#define DIGITVIEWER_H

#include <QWidget>

class DigitViewer : public QWidget
{
    Q_OBJECT

    static const int squareSize = 5;
    char* data;

    virtual void paintEvent (QPaintEvent* );

public:
    static const int dimension = 784;
    static const int side = 28;

    explicit DigitViewer(char* , QWidget *parent = 0 );
    explicit DigitViewer(QWidget *parent = 0 );
    virtual QSize sizeHint () const;
    void changeData(double *digits);
    ~DigitViewer();

signals:

public slots:
};

#endif // DIGITVIEWER_H
