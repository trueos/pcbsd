
QT       += core network

TARGET=pcbsd-utils
target.path = /usr/local/lib

DESTDIR= $$_PRO_FILE_PWD_/../

TEMPLATE	= lib
LANGUAGE	= C++
VERSION		= 0.9.8

HEADERS	+= pcbsd-netif.h \
	pcbsd-utils.h \
        pcbsd-hardware.h 

SOURCES	+= utils.cpp \
        hardware.cpp \
        netif.cpp

include.path=/usr/local/include/
include.files=pcbsd-*.h

INSTALLS += target include 

QMAKE_LIBDIR = /usr/local/lib/qt4 /usr/local/lib
