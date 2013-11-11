#-------------------------------------------------
#
# Project created by QtCreator 2013-11-08T13:06:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS	+= -L../libpcbsd -L/usr/local/lib -lpcbsd-ui -lpcbsd-utils -lQtSolutions_SingleApplication-head
INCLUDEPATH	+= ../libpcbsd/utils ../libpcbsd/ui /usr/local/include


TARGET = pc-updatecenter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    indicatorwidget.cpp \
    updatecontroller.cpp \
    syscontroller.cpp \
    pkgcontroller.cpp \
    pbicontroller.cpp

HEADERS  += mainwindow.h \
    indicatorwidget.h \
    updatecontroller.h \
    syscontroller.h \
    pkgcontroller.h \
    pbicontroller.h

FORMS    += mainwindow.ui \
    indicatorwidget.ui

RESOURCES += \
    res.qrc
