#-------------------------------------------------
#
# Project created by QtCreator 2013-11-29T17:58:23
#
#-------------------------------------------------



QT       += core gui

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -w

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server
TEMPLATE = app


SOURCES += src/main.cpp \
    src/gui/mainwindow.cpp \
    src/network/http/server.cpp \
    src/network/http/script_manager.cpp \
    src/network/http/script.cpp \
    src/network/http/request.cpp \
    src/network/http/reply.cpp \
    src/network/http/client.cpp

HEADERS  += \
    evse/gui/mainwindow.hpp \
    evse/network/tcp/http/server.hpp \
    evse/network/tcp/http/script_manager.hpp \
    evse/network/tcp/http/script.hpp \
    evse/network/tcp/http/request.hpp \
    evse/network/tcp/http/reply.hpp \
    evse/network/tcp/http/client.hpp \
    evse/network/tcp/tcp_server.hpp \
    evse/network/tcp/tcp_socket.hpp \
    evse/std_and_boost.hpp

FORMS    += utils/mainwindow.ui


linux-g++{
    BOOST_DIR = /usr/local/boost_x64
}

linux-rasp-pi-g++{
    BOOST_DIR = /usr/local/boost_arm
}

BOOST_DIR_LIBS = $$BOOST_DIR/lib/

unix:!macx: LIBS += $$BOOST_DIR_LIBS/libboost_system.a

# Boost Ordner
INCLUDEPATH += $$BOOST_DIR

#Projekt Ordner auch als Include-Pfad verwenden
INCLUDEPATH += $$PWD

#Abhängigkeiten
DEPENDPATH += $$BOOST_DIR
