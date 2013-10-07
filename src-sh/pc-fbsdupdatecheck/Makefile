#############################################################################
# Makefile for building: $(PREFIX)/bin/pc-fbsdupdatecheck
#############################################################################
PREFIX?= /usr/local

####### Compiler, tools and options

CC            = cc
CXX           = c++
DEFINES       = -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -O2 -Wall -W $(DEFINES)
CXXFLAGS      = -pipe -O2 -Wall -W $(DEFINES)
LINK          = c++
LFLAGS        = -Wl,-O1 -pthread
LIBS          = $(SUBLIBS)
AR            = ar cqs
RANLIB        = 
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -R
STRIP         = 
INSTALL_FILE  = $(COPY_FILE)
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = $(COPY_FILE)
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.c 
OBJECTS       = main.o
MAKE_TARGET   = pc-fbsdupdatecheck
DESTDIR       = $(PREFIX)/bin/
TARGET        = pc-fbsdupdatecheck

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	@$(CHK_DIR_EXISTS) $(PREFIX)/bin/ || $(MKDIR) $(PREFIX)/bin/ 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile:

dist: 
	@$(CHK_DIR_EXISTS) .tmp/pc-fbsdupdatecheck1.0.0 || $(MKDIR) .tmp/pc-fbsdupdatecheck1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/pc-fbsdupdatecheck1.0.0/ && $(COPY_FILE) --parents updatecheck.c .tmp/pc-fbsdupdatecheck1.0.0/ && (cd `dirname .tmp/pc-fbsdupdatecheck1.0.0` && $(TAR) pc-fbsdupdatecheck1.0.0.tar pc-fbsdupdatecheck1.0.0 && $(COMPRESS) pc-fbsdupdatecheck1.0.0.tar) && $(MOVE) `dirname .tmp/pc-fbsdupdatecheck1.0.0`/pc-fbsdupdatecheck1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/pc-fbsdupdatecheck1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all:
compiler_moc_header_clean:
compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: 

####### Compile

main.o: main.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o main.o main.c

####### Install

install_scripts: first FORCE
	-@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)$(PREFIX)/bin/ || $(MKDIR) $(INSTALL_ROOT)$(PREFIX)/bin/
	-$(INSTALL_FILE) pc-fbsdupdatecheck $(INSTALL_ROOT)$(PREFIX)/bin/

install:   FORCE install_scripts

uninstall:   FORCE

FORCE:

