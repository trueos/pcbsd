#
#	Makefile
#

.include <bsd.own.mk>

PREFIX?= /usr/local
MAN=
BINOWN=		root
BINGRP=		wheel
BINMODE=	0555
DESTDIR=$(PREFIX)/bin
FILESDIR=$(PREFIX)/bin
MKDIR=mkdir

PROG=	pc-nssconf
SRCS=	pc-nssconf.c pc-nssconf.h

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

OBJS+=	nss-parser.tab.o nss-lexer.lex.o

CLEANFILES+= nss-parser.tab.o nss-lexer.lex.o \
	nss-parser.tab.c nss-parser.tab.h nss-lexer.lex.c \
	nss-parser.tab.i

nss-lexer.tab.o: nss-parser.tab.c nss-parser.tab.h
nss-parser.tab.o: nss-lexer.lex.c nss-parser.tab.h

nss-parser.tab.c: nss-parser.y
	${BISON} -o$@ -d nss-parser.y

nss-parser.tab.h: nss-parser.tab.c

nss-lexer.lex.c: nss-lexer.l
	${LEX} ${LEXFLAGS} -o$@ nss-lexer.l

install:
	$(MKDIR) -p $(FILESDIR)
	$(INSTALL) -m $(BINMODE) $(PROG) $(FILESDIR)/
	strip $(FILESDIR)/$(PROG)

.include <bsd.prog.mk>
