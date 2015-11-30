TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release
QT = core network

HEADERS	+= MainClient.h
		
SOURCES	+= main.cpp \
		MainClient.cpp


TARGET=pc-sysconfig
target.path=/usr/local/bin


MANPAGES.extra="gzip -c pc-sysconfig.8 > pc-sysconfig.8.gz ; mv -f pc-sysconfig.8.gz $(INSTALL_ROOT)/usr/local/man/man8/pc-sysconfig.8.gz"
MANPAGES.path=/usr/local/man/man8

INSTALLS += target MANPAGES


QMAKE_LIBDIR = /usr/local/lib/qt5 /usr/local/lib
