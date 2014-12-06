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
    dialogs/dialogconflict.cpp \
    sysdetailswidget.cpp \
    pkgdetailswidget.cpp \
    pbidetailswidget.cpp \
    dialogs/logviewdialog.cpp \
    rebootwidget.cpp \
    dialogs/jailsdialog.cpp \
    jailsbackend.cpp \
    jailindicator.cpp \
    dialogs/branchesdialog.cpp \
    dialogs/patchsetdialog.cpp \
    dialogs/updatehistorydialog.cpp

HEADERS  += mainwindow.h \
    indicatorwidget.h \
    updatecontroller.h \
    syscontroller.h \
    pkgcontroller.h \
    pbicontroller.h \
    dialogs/dialogconflict.h \
    utils.h \
    sysdetailswidget.h \
    pkgdetailswidget.h \
    pbidetailswidget.h \
    dialogs/logviewdialog.h \
    rebootwidget.h \
    dialogs/jailsdialog.h \
    jailsbackend.h \
    jailindicator.h \
    dialogs/branchesdialog.h \
    dialogs/patchsetdialog.h \
    dialogs/updatehistorydialog.h

FORMS    += mainwindow.ui \
    indicatorwidget.ui \
    dialogs/dialogconflict.ui \
    sysdetailswidget.ui \
    pkgdetailswidget.ui \
    pbidetailswidget.ui \
    dialogs/logviewdialog.ui \
    rebootwidget.ui \
    dialogs/jailsdialog.ui \
    jailindicator.ui \
    dialogs/branchesdialog.ui \
    dialogs/patchsetdialog.ui \
    dialogs/updatehistorydialog.ui

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
