#-------------------------------------------------
#
# Project created by QtCreator 2014-05-15T11:11:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pc-controlpanel
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    backend/cp-item.cpp

HEADERS  += mainwindow.h \
    backend/cp-item.h

FORMS    += mainwindow.ui
