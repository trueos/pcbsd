TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release
QT = core network websockets

HEADERS	+= WebServer.h \
		WebSocket.h \
		syscache-client.h \
		dispatcher-client.h \
		RestStructs.h \
		AuthorizationManager.h
		
SOURCES	+= main.cpp \
		WebServer.cpp \
		WebSocket.cpp \
		syscache-client.cpp \
		dispatcher-client.cpp \
		AuthorizationManager.cpp


TARGET=syscache-webclient
target.path=/usr/local/bin


INSTALLS += target


QMAKE_LIBDIR = /usr/local/lib/qt5 /usr/local/lib

INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lpam -lutil