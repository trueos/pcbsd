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

PROG=	pc-sambaconf
SRCS=	pc-sambaconf.c pc-sambaconf.h

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

OBJS+=	samba-parser.tab.o samba-lexer.lex.o

CLEANFILES+= samba-parser.tab.o samba-lexer.lex.o \
	samba-parser.tab.c samba-parser.tab.h samba-lexer.lex.c \
	samba-parser.tab.i

samba-lexer.tab.o: samba-parser.tab.c samba-parser.tab.h
samba-parser.tab.o: samba-lexer.lex.c samba-parser.tab.h

samba-parser.tab.c: samba-parser.y
	${BISON} -o$@ -d samba-parser.y

samba-parser.tab.h: samba-parser.tab.c

samba-lexer.lex.c: samba-lexer.l
	${LEX} ${LEXFLAGS} -o$@ samba-lexer.l

install:
	$(MKDIR) -p $(FILESDIR)
	$(INSTALL) -m $(BINMODE) $(PROG) $(FILESDIR)/
	strip $(FILESDIR)/$(PROG)

.include <bsd.prog.mk>
