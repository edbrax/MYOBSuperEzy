#-------------------------------------------------
#
# Project created by QtCreator 2017-01-15T20:05:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MLCSuperEzy
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
<<<<<<< HEAD
    dataformatcontroller.cpp \
    outputfilecontroller.cpp \
    datafilecontroller.cpp

HEADERS  += mainwindow.h \
    dataformatcontroller.h \
    outputfilecontroller.h \
    datafilecontroller.h
=======
    inputfilecontroller.cpp

HEADERS  += mainwindow.h \
    inputfilecontroller.h
>>>>>>> Issue2

FORMS    += mainwindow.ui
