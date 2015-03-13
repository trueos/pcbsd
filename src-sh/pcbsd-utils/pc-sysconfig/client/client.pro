TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release
QT = core network

HEADERS	+= MainClient.h
		
SOURCES	+= main.cpp \
		MainClient.cpp


TARGET=pc-sysconfig
target.path=/usr/local/bin


INSTALLS += target


QMAKE_LIBDIR = /usr/local/lib/qt5 /usr/local/lib
