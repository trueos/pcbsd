#-------------------------------------------------
#
# Project created by QtCreator 2011-04-19T11:05:08
#
#-------------------------------------------------

QT       += core gui

TARGET = pc-bugreport
target.path = /usr/local/bin

TEMPLATE = app

LIBS +=-L../libpcbsd -L/usr/local/lib -lpcbsd-ui -lpcbsd-utils
INCLUDEPATH	+= ../libpcbsd/ui/ ../libpcbsd/utils/ /usr/local/include

SOURCES += main.cpp\
        mainwindow.cpp \
    maillist.cpp \
    toolbox.cpp \
    showinfodialog.cpp \
    weburl.cpp \
    pbiinfo.cpp

HEADERS  += mainwindow.h \
    maillist.h \
    report_places.h \
    toolbox.h \
    showinfodialog.h \
    weburl.h \
    pbiinfo.h

FORMS    += mainwindow.ui \
    maillist.ui \
    toolbox.ui \
    showinfodialog.ui \
    weburl.ui

RESOURCES += \
    bugreport.qrc


desktop.path=/usr/local/share/applications/
desktop.files=EasyPBI.desktop

icons.path=/usr/local/share/pc-bugreport/
icons.files=icons/pc-bugreport.png

INSTALLS += target desktop icons
