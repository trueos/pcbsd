
QT       += core

TARGET=LuminaUtils
target.path = /usr/local/lib

DESTDIR= $$_PRO_FILE_PWD_/

TEMPLATE	= lib
LANGUAGE	= C++
VERSION		= 1

HEADERS	+= LuminaXDG.h \
	LuminaUtils.h \
	LuminaX11.h

SOURCES	+= LuminaXDG.cpp \
	LuminaUtils.cpp \
	LuminaX11.cpp

INCLUDEPATH += /usr/local/include

LIBS	+= -lX11 -lXrender

include.path=/usr/local/include/
include.files=LuminaXDG.h \
	LuminaUtils.h \
	LuminaX11.h \

INSTALLS += target include 

QMAKE_LIBDIR = /usr/local/lib/qt4 /usr/local/lib
