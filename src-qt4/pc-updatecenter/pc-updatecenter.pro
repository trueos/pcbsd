#-------------------------------------------------
#
# Project created by QtCreator 2013-11-08T13:06:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pc-updatecenter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    indicatorwidget.cpp \
    updatecontroller.cpp \
    syscontroller.cpp

HEADERS  += mainwindow.h \
    indicatorwidget.h \
    updatecontroller.h \
    syscontroller.h

FORMS    += mainwindow.ui \
    indicatorwidget.ui

RESOURCES += \
    res.qrc
