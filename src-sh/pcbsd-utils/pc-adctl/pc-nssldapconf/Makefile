#
#	Makefile
#

.include <bsd.own.mk>

PREFIX?= /usr/local
MAN=
BINOWN=		root
BINGRP=		wheel
BINMODE=	0555
DESTDIR=	$(PREFIX)/bin
MKDIR=		mkdir

PROG=	pc-nssldapconf
SRCS=	pc-nssldapconf.c pc-nssldapconf.h

CFLAGS+= -I. -I${.CURDIR}

MAN=
DEBUG?=	0

BISON=	yacc
LEX=	lex

.if ${DEBUG} == 1
LEXFLAGS= -d
CFLAGS+=-g -ggdb -g3
.else
LEXFLAGS=
.endif

OBJS+=	nssldap-parser.tab.o nssldap-lexer.lex.o

CLEANFILES+= nssldap-parser.tab.o nssldap-lexer.lex.o \
	nssldap-parser.tab.c nssldap-parser.tab.h nssldap-lexer.lex.c \
	nssldap-parser.tab.i

nssldap-lexer.tab.o: nssldap-parser.tab.c nssldap-parser.tab.h
nssldap-parser.tab.o: nssldap-lexer.lex.c nssldap-parser.tab.h

nssldap-parser.tab.c: nssldap-parser.y
	${BISON} -o$@ -d nssldap-parser.y

nssldap-parser.tab.h: nssldap-parser.tab.c

nssldap-lexer.lex.c: nssldap-lexer.l
	${LEX} ${LEXFLAGS} -o$@ nssldap-lexer.l

install:
	$(MKDIR) -p $(DESTDIR)
	$(INSTALL) -m $(BINMODE) $(PROG) $(DESTDIR)/

.include <bsd.prog.mk>
