#############################################################################
# Makefile for building: pcbsd-utils
#############################################################################

PREFIX?= /usr/local

####### Install

all: domake

domake:
	sh install.sh DOMAKE

install_doinstall:
	sh install.sh $(PREFIX)

install:  domake install_doinstall
