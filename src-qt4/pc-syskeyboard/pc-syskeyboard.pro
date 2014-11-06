#-------------------------------------------------
#
# Project created by QtCreator 2014-11-06T01:16:21
#
#-------------------------------------------------

QT       += core gui

LIBS	 += -L../libpcbsd -L/usr/local/lib -lpcbsd-ui -lpcbsd-utils -lQtSolutions_SingleApplication-head
INCLUDEPATH	+= ../libpcbsd/utils ../libpcbsd/ui /usr/local/include

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pc-syskeyboard
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    keyboardsettings.cpp \
    dialogs/kblayoutselectdialog.cpp

HEADERS  += mainwindow.h \
    keyboardsettings.h \
    dialogs/kblayoutselectdialog.h

FORMS    += mainwindow.ui \
    dialogs/kblayoutselectdialog.ui

RESOURCES += \
    syskeyboard.qrc
