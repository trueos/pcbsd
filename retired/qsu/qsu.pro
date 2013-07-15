TEMPLATE     = app
TARGET       = qsu
DEPENDPATH  += resources src
INCLUDEPATH += src

# PREFIX
isEmpty(PREFIX) { PREFIX = /usr/local }

# UI
FORMS       += src/passwordpromptdialog.ui
HEADERS     += src/passwordpromptdialog.h
SOURCES     += src/passwordpromptdialog.cpp

# BASE
HEADERS     += src/database.h src/session.h
SOURCES     += src/main.c src/database.c src/strings_en.c src/conversation.cpp
RESOURCES   += resources/qsu.qrc

# DEPS
LIBS        += -lpam

# INSTALL
target.path  = $$PREFIX/bin
INSTALLS    += target
QMAKE_INSTALL_PROGRAM = install -m 4711 -p -o root

# REQUIRED
prep.path    = $$PREFIX/var/db/qsu
prep.files   = $$PWD/build/.keep
INSTALLS    += prep

# DEFAULT CONFIG
!exists( config.h ) {
	system(cp $$PWD/config.h.def $$OUT_PWD/config.h)
}
