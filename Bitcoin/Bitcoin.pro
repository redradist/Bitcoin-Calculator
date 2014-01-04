#-------------------------------------------------
#
# Project created by QtCreator 2013-12-25T03:47:29
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BitCoin
TEMPLATE = app

CONFIG += release
CONFIG += static

QMAKE_LFLAGS += -static -static-libgcc
LIBS += -static-libgcc

SOURCES += main.cpp\
        mainwindow.cpp \
    network.cpp \
    miners.cpp \
    calcwindow.cpp \
    help.cpp

HEADERS  += mainwindow.h \
    network.h \
    miners.h \
    constans.h \
    calcwindow.h \
    help.h

FORMS    += mainwindow.ui \
    mainwindow.ui \
    calcwindow.ui \
    help.ui

RESOURCES += \
    imageS.qrc
