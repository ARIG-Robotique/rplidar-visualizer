#-------------------------------------------------
#
# Project created by QtCreator 2016-11-23T17:48:14
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RPLidarVisualizer
TEMPLATE = app

SOURCES += main.cpp \
    mainwindow.cpp \
    scanthread.cpp \
    lidardataviewwindow.cpp

HEADERS += \
    mainwindow.h \
    scanthread.h \
    lidardataviewwindow.h

FORMS += \
    mainwindow.ui

DISTFILES +=

unix:!macx: LIBS += -L$$PWD/libs/rplidar/ -lrplidar_sdk

INCLUDEPATH += $$PWD/libs/rplidar/include
DEPENDPATH += $$PWD/libs/rplidar/include

unix:!macx: PRE_TARGETDEPS += $$PWD/libs/rplidar/librplidar_sdk.a
