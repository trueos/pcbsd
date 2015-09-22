TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release
QT = core network websockets

HEADERS	+= WebServer.h \
		WebSocket.h \
		syscache-client.h \
		RestStructs.h
		
SOURCES	+= main.cpp \
		WebServer.cpp \
		WebSocket.cpp \
		syscache-client.cpp


TARGET=syscache-webclient
target.path=/usr/local/bin


INSTALLS += target


QMAKE_LIBDIR = /usr/local/lib/qt5 /usr/local/lib
LIBS += -L/usr/local/lib