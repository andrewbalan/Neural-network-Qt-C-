#-------------------------------------------------
#
# Project created by QtCreator 2015-01-18T19:40:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NEURAL_NET
TEMPLATE = app


SOURCES += main.cpp\
        readmnist.cpp \
    digitviewer.cpp \
    perceptron.cpp \
    layer.cpp \
    mainview.cpp \
    datasource.cpp

HEADERS  += readmnist.h \
    digitviewer.h \
    perceptron.h \
    layer.h \
    mainview.h \
    datasource.h

FORMS    += readmnist.ui \
    layerview.ui \
    mainview.ui

RESOURCES +=
