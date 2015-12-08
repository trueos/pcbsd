
QT       += core network

TARGET=sysadm
target.path = /usr/local/lib

DESTDIR= $$_PRO_FILE_PWD_

TEMPLATE	= lib
LANGUAGE	= C++
VERSION		= 1.0.0

HEADERS	+= sysadm-global.h \
		sysadm-general.h \
		sysadm-network.h
		

SOURCES	+= sysadm-general.cpp \
		sysadm-network.cpp \
		NetDevice.cpp

include.path=/usr/local/include/
include.files=sysadm-*.h

INSTALLS += target include 

QMAKE_LIBDIR = /usr/local/lib/qt5 /usr/local/lib
