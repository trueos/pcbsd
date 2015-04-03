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

PROG=	pc-ldapconf
SRCS=	pc-ldapconf.c pc-ldapconf.h

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

OBJS+=	ldap-parser.tab.o ldap-lexer.lex.o

CLEANFILES+= ldap-parser.tab.o ldap-lexer.lex.o \
	ldap-parser.tab.c ldap-parser.tab.h ldap-lexer.lex.c \
	ldap-parser.tab.i

ldap-lexer.tab.o: ldap-parser.tab.c ldap-parser.tab.h
ldap-parser.tab.o: ldap-lexer.lex.c ldap-parser.tab.h

ldap-parser.tab.c: ldap-parser.y
	${BISON} -o$@ -d ldap-parser.y

ldap-parser.tab.h: ldap-parser.tab.c

ldap-lexer.lex.c: ldap-lexer.l
	${LEX} ${LEXFLAGS} -o$@ ldap-lexer.l

install:
	$(MKDIR) -p $(FILESDIR)
	$(INSTALL) -m $(BINMODE) $(PROG) $(FILESDIR)/

.include <bsd.prog.mk>
