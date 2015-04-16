TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release
QT = core network

HEADERS	+= MainDaemon.h \
			Backend.h \
			DevDB.h
			
SOURCES	+= main.cpp \
		MainDaemon.cpp \
		Backend-remdev.cpp \
		Backend-audio.cpp \
		Backend-screen.cpp


TARGET=pc-sysconfig-daemon
target.path=/usr/local/bin


INSTALLS += target


QMAKE_LIBDIR = /usr/local/lib/qt5 /usr/local/lib
