TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release
QT		+= core network

HEADERS	+= syscache-daemon.h
		
SOURCES	+= main.cpp \
		syscache-daemon.cpp


TARGET=syscache-daemon
target.path=/usr/local/bin


INSTALLS += target


QMAKE_LIBDIR = /usr/local/lib/qt4 /usr/local/lib
