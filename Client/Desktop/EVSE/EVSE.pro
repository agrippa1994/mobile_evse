#-------------------------------------------------
#
# Project created by QtCreator 2014-02-24T12:03:21
#
#-------------------------------------------------

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -w

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EVSE
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
