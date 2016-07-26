#include "digitviewer.h"
#include "QDebug"
#include "QPainter"


DigitViewer::DigitViewer(char* digits, QWidget *parent  ) : QWidget(parent)
{
    setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
    data = digits;
}

DigitViewer::DigitViewer(QWidget *parent)  : QWidget(parent)
{
    setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed);
    data = new char[dimension];
    for (int i = 0; i < dimension; ++i) {
        data[i] = 5;
    }
}

void DigitViewer::paintEvent(QPaintEvent *)
{
    QPainter ptr(this);

    ptr.fillRect(rect(), Qt::white);
    ptr.setPen(QColor(50, 50, 50, 0));

    for (int row = 0; row < side; ++row) {
        for (int col = 0; col < side; ++col) {
            unsigned char alpha = (unsigned char) data[row * side + col];
            ptr.setBrush ( QBrush(QColor(0, 0, 0, alpha)) );

            int x = row * squareSize;
            int y = col   * squareSize;
            ptr.drawRect (y, x, squareSize, squareSize);
        }
    }
}

QSize DigitViewer::sizeHint() const
{
    int width = side * squareSize;
    return(QSize(width + 1, width + 1));
}

void DigitViewer::changeData(double* digits)
{
    for (int i = 0; i < dimension; ++i) {
        data[i] = digits[i];
    }
    repaint();
}

DigitViewer::~DigitViewer()
{
    delete[] data;
}

