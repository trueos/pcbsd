TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release
QT = core network

HEADERS	+= syscache-client.h
		
SOURCES	+= main.cpp \
		syscache-client.cpp


TARGET=syscache
target.path=/usr/local/bin


INSTALLS += target


QMAKE_LIBDIR = /usr/local/lib/qt5 /usr/local/lib
