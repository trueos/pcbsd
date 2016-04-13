TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release
QT = core network

HEADERS	+= syscache-daemon.h \
			DB.h
		
SOURCES	+= main.cpp \
		syscache-daemon.cpp \
		DB.cpp


TARGET=syscache-daemon
target.path=/usr/local/bin


INSTALLS += target


QMAKE_LIBDIR = /usr/local/lib/qt5 /usr/local/lib
LIBS += -L/usr/local/lib