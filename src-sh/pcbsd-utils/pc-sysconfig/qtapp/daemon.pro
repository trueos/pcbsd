TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release
QT = core

HEADERS	+= Backend.h \
			DevDB.h
			
SOURCES	+= main.cpp \
		Backend-remdev.cpp \
		Backend-audio.cpp \
		Backend-screen.cpp \
		Backend-network.cpp


TARGET=pc-sysconfig
target.path=/usr/local/bin

#The binary needs to be setuid root for system access
perms.path=/usr/local/bin
perms.extra="chmod 4555 /usr/local/bin/pc-sysconfig";

#Also install the man page
MANPAGES.extra="gzip -c pc-sysconfig.8 > pc-sysconfig.8.gz ; mv -f pc-sysconfig.8.gz $(INSTALL_ROOT)/usr/local/man/man8/pc-sysconfig.8.gz"
MANPAGES.path=/usr/local/man/man8

INSTALLS += target perms MANPAGES


QMAKE_LIBDIR = /usr/local/lib/qt5 /usr/local/lib
