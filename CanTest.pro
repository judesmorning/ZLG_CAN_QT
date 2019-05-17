#-------------------------------------------------
#
# Project created by QtCreator 2019-04-08T11:04:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CanTest
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    mycanthread.cpp \
    myparse.cpp \
    mymethod.cpp \
    canmanager.cpp \
    makepacket.cpp \
    statepacket.cpp \
    packemanager.cpp \
    mytimer.cpp \
    gatherpacket.cpp \
    controlpacket.cpp \
    testpacket.cpp \
    infopacket.cpp

HEADERS  += mainwindow.h \
    ControlCAN.h \
    mycanthread.h \
    includes.h \
    myparse.h \
    mymethod.h \
    canmanager.h \
    makepacket.h \
    statepacket.h \
    packemanager.h \
    mytimer.h \
    gatherpacket.h \
    controlpacket.h \
    testpacket.h \
    infopacket.h

FORMS    += mainwindow.ui \
    makepacket.ui \
    statepacket.ui \
    gatherpacket.ui \
    controlpacket.ui \
    testpacket.ui \
    infopacket.ui

win32: LIBS += -L$$PWD/lib/ -lControlCAN

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

CONFIG += c++11
