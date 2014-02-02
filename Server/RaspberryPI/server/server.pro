#-------------------------------------------------
#
# Project created by QtCreator 2013-11-29T17:58:23
#
#-------------------------------------------------

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -w

TARGET = server
TEMPLATE = app

CONFIG += console

SOURCES += src/main.cpp \
    src/network/io_service.cpp \
    src/evse.cpp \
    src/network/usb/usb.cpp

HEADERS  += \
    evse/network/tcp/tcp_server.hpp \
    evse/network/tcp/tcp_socket.hpp \
    evse/network/io_service.hpp \
    evse/evse.hpp \
    evse/network/usb/usb.hpp


#x64 - Linux
linux-g++
{
    BOOST_DIR = /usr/local/boost_x64
}

#Rasperry PI
linux-rasp-pi-g++
{
    target.path = /home/pi/server
    TARGET = server
    INSTALLS += target

    DEFINES += RASPBERRY_PI

    BOOST_DIR = /usr/local/boost_arm
}

BOOST_DIR_LIBS = $$BOOST_DIR/lib/

unix:!macx: LIBS += -lrt
unix:!macx: LIBS += $$BOOST_DIR_LIBS/libboost_system.a
unix:!macx: LIBS += $$BOOST_DIR_LIBS/libboost_thread.a
unix:!macx: LIBS += $$BOOST_DIR_LIBS/libboost_filesystem.a
unix:!macx: LIBS += $$BOOST_DIR_LIBS/libboost_log.a

# Boost Ordner
INCLUDEPATH += $$BOOST_DIR

#Projekt Ordner auch als Include-Pfad verwenden
INCLUDEPATH += $$PWD

#Abhängigkeiten
DEPENDPATH += $$BOOST_DIR
