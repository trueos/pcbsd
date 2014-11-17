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

PROG=	pc-pamconf
SRCS=	pc-pamconf.c pc-pamconf.h

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

OBJS+=	pam-parser.tab.o pam-lexer.lex.o

CLEANFILES+= pam-parser.tab.o pam-lexer.lex.o \
	pam-parser.tab.c pam-parser.tab.h pam-lexer.lex.c \
	pam-parser.tab.i

pam-lexer.tab.o: pam-parser.tab.c pam-parser.tab.h
pam-parser.tab.o: pam-lexer.lex.c pam-parser.tab.h

pam-parser.tab.c: pam-parser.y
	${BISON} -o$@ -d pam-parser.y

pam-parser.tab.h: pam-parser.tab.c

pam-lexer.lex.c: pam-lexer.l
	${LEX} ${LEXFLAGS} -o$@ pam-lexer.l

.include <bsd.prog.mk>
