
QT       += core network

TARGET=pc-coreutils
target.path = /usr/local/lib

DESTDIR= $$_PRO_FILE_PWD_

TEMPLATE	= lib
LANGUAGE	= C++
VERSION		= 1.0.0

HEADERS	+= pcbsd-general.h \
		pcbsd-network.h
		

SOURCES	+= pcbsd-general.cpp \
		pcbsd-network.cpp

include.path=/usr/local/include/
include.files=pcbsd-*.h

INSTALLS += target include 

QMAKE_LIBDIR = /usr/local/lib/qt5 /usr/local/lib
