#-------------------------------------------------
#
# Project created by QtCreator 2014-02-24T12:03:21
#
#-------------------------------------------------

QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -w

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EVSE
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    evsewindow.cpp \
    servermanager.cpp

HEADERS  += mainwindow.h \
    evsewindow.h \
    servermanager.h

FORMS    += mainwindow.ui \
    evsewindow.ui \
    servermanager.ui
