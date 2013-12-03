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
target.path=/usr/local/bin

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

OTHER_FILES += \
    pc-updatecenter.desktop

doicons.path=/usr/local/share/pcbsd/icons/
doicons.files=update-center.png

desktop.path=/usr/local/share/applications/
desktop.files=pc-updatecenter.desktop

dotrans.path=/usr/local/share/pcbsd/i18n/
#dotrans.extra=cd i18n && lrelease-qt4 -nounfinished *.ts && cp *.qm $(INSTALL_ROOT)/usr/local/share/pcbsd/i18n/


INSTALLS+= target doicons desktop dotrans
