TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release
QT = core
		
SOURCES	+= main.cpp 

LIBS = -L../library -L/usr/lib -L/usr/local/lib -lsysadm
INCLUDEPATH += ../library /usr/include /usr/local/include

TARGET=sysadm
target.path=/usr/local/bin


INSTALLS += target


QMAKE_LIBDIR = /usr/local/lib/qt5 /usr/local/lib
