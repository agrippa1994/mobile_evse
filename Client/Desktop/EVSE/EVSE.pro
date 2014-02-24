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

#x64 - Linux

    BOOST_DIR = /usr/local/boost_x64



BOOST_DIR_LIBS = $$BOOST_DIR/lib

unix:!macx: LIBS += -lrt
unix:!macx: LIBS += $$BOOST_DIR_LIBS/libboost_system.a


# Boost Ordner
INCLUDEPATH += $$BOOST_DIR

#Projekt Ordner auch als Include-Pfad verwenden
INCLUDEPATH += $$PWD

#Abhängigkeiten
DEPENDPATH += $$BOOST_DIR
