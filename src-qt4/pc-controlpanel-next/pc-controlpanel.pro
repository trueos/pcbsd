#-------------------------------------------------
#
# Project created by QtCreator 2014-05-15T11:11:50
#
#-------------------------------------------------

QT       += core gui

LIBS	 += -L../libpcbsd -L/usr/local/lib -lpcbsd-ui -lpcbsd-utils -lQtSolutions_SingleApplication-head
INCLUDEPATH	+= ../libpcbsd/utils ../libpcbsd/ui /usr/local/include

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pc-controlpanel
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    backend/cp-item.cpp

HEADERS  += mainwindow.h \
    backend/cp-item.h

FORMS    += mainwindow.ui


desktop.path=/usr/local/share/applications/
desktop.files=pccontrol.desktop

cleanitems.path=/usr/local/share/pcbsd/pc-controlpanel/items
cleanitems.extra=rm -rf $(INSTALL_ROOT)/usr/local/share/pcbsd/pc-controlpanel/items

mkdiritems.path=/usr/local/share/pcbsd/pc-controlpanel/items
mkdiritems.extra=mkdir -p $(INSTALL_ROOT)/usr/local/share/pcbsd/pc-controlpanel/items

cpitems.path=/usr/local/share/pcbsd/pc-controlpanel/items
cpitems.extra=tar cvf - --exclude '.svn/' -C items . 2>/dev/null | tar xvf - -C $(INSTALL_ROOT)/usr/local/share/pcbsd/pc-controlpanel/items 2>/dev/null

mkdiricons.path=/usr/local/share/pcbsd/pc-controlpanel/icons
mkdiricons.extra=mkdir -p $(INSTALL_ROOT)/usr/local/share/pcbsd/pc-controlpanel/icons

cpicons.path=/usr/local/share/pcbsd/pc-controlpanel/items
cpicons.extra=tar cvf - --exclude '.svn/' -C icons . 2>/dev/null | tar xvf - -C $(INSTALL_ROOT)/usr/local/share/pcbsd/pc-controlpanel/icons 2>/dev/null

INSTALLS += target desktop cleanitems mkdiritems cpitems mkdiricons cpicons