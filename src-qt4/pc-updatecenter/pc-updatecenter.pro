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
    pbicontroller.cpp \
    dialogconflict.cpp \
    sysdetailswidget.cpp \
    pkgdetailswidget.cpp \
    pbidetailswidget.cpp \
    logviewdialog.cpp

HEADERS  += mainwindow.h \
    indicatorwidget.h \
    updatecontroller.h \
    syscontroller.h \
    pkgcontroller.h \
    pbicontroller.h \
    dialogconflict.h \
    utils.h \
    sysdetailswidget.h \
    iconset.h \
    pkgdetailswidget.h \
    pbidetailswidget.h \
    logviewdialog.h

FORMS    += mainwindow.ui \
    indicatorwidget.ui \
    dialogconflict.ui \
    sysdetailswidget.ui \
    pkgdetailswidget.ui \
    pbidetailswidget.ui \
    logviewdialog.ui

RESOURCES += \
    res.qrc
