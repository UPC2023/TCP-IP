#-------------------------------------------------
#
# Project created by QtCreator 2018-12-04T00:24:32
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TCPClient
TEMPLATE = app


SOURCES += main.cpp\
        mywidget.cpp \
    listwidget.cpp \
    network.cpp

HEADERS  += mywidget.h \
    messageheader.h

FORMS    += mywidget.ui

CONFIG += C++11

RESOURCES += \
    res/pic/pic.qrc \
    res/qss/qss.qrc
