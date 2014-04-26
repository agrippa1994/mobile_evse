#-------------------------------------------------
#
# Project created by QtCreator 2013-11-29T17:58:23
#
#-------------------------------------------------

QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -w

QT -= core gui widget

win32{
    QT += core
}

TARGET = server
TEMPLATE = app

CONFIG += console

SOURCES += main.cpp \
    usb_base.cpp \
    arduino.cpp \
    tcp_client_base.cpp \
    tcp_server_base.cpp \
    network.cpp

HEADERS += \
    usb_base.h \
    arduino.h \
    tcp_client_base.h \
    tcp_server_base.h \
    network.h \
    log.h


#Rasperry PI
linux-rasp-pi-g++
{
    target.path = /home/pi/server
    TARGET = server
    INSTALLS += target

    DEFINES += RASPBERRY_PI

    BOOST_DIR = /usr/local/boost_arm
}


#BOOST_DIR = /usr/local/boost_x64
BOOST_DIR_LIBS = $$BOOST_DIR/lib/

unix:!macx: LIBS += $$BOOST_DIR_LIBS/libboost_log.a
unix:!macx: LIBS += $$BOOST_DIR_LIBS/libboost_filesystem.a
unix:!macx: LIBS += $$BOOST_DIR_LIBS/libboost_thread.a
unix:!macx: LIBS += $$BOOST_DIR_LIBS/libboost_system.a
unix:!macx: LIBS += -lpthread
unix:!macx: LIBS += -lrt

win32: LIBS += C:/Qt/Qt5.2.1/Tools/mingw48_32/i686-w64-mingw32/lib/libboost_log-mgw48-mt-1_55.a
win32: LIBS += C:/Qt/Qt5.2.1/Tools/mingw48_32/i686-w64-mingw32/lib/libboost_log_setup-mgw48-mt-1_55.a
win32: LIBS += C:/Qt/Qt5.2.1/Tools/mingw48_32/i686-w64-mingw32/lib/libboost_filesystem-mgw48-mt-1_55.a
win32: LIBS += C:/Qt/Qt5.2.1/Tools/mingw48_32/i686-w64-mingw32/lib/libboost_thread-mgw48-mt-1_55.a
win32: LIBS += C:/Qt/Qt5.2.1/Tools/mingw48_32/i686-w64-mingw32/lib/libboost_system-mgw48-mt-1_55.a
win32: LIBS += C:/Qt/Qt5.2.1/Tools/mingw48_32/i686-w64-mingw32/lib/libws2_32.a
win32: LIBS += C:/Qt/Qt5.2.1/Tools/mingw48_32/i686-w64-mingw32/lib/libmswsock.a

# Boost Ordner
INCLUDEPATH += $$BOOST_DIR

#Projekt Ordner auch als Include-Pfad verwenden
INCLUDEPATH += $$PWD

#Abhängigkeiten
DEPENDPATH += $$BOOST_DIR
