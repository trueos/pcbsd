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

PROG=	pc-krbconf
SRCS=	pc-krbconf.c pc-krbconf.h

CFLAGS+= -I. -I${.CURDIR}

MAN=
DEBUG=	0

BISON=	yacc
LEX=	lex

.if ${DEBUG} == 1
LEXFLAGS= -d
.else
LEXFLAGS=
.endif

OBJS+=	krb5-parser.tab.o krb5-lexer.lex.o

CLEANFILES+= krb5-parser.tab.o krb5-lexer.lex.o \
	krb5-parser.tab.c krb5-parser.tab.h krb5-lexer.lex.c \
	krb5-parser.tab.i

krb5-lexer.tab.o: krb5-parser.tab.c krb5-parser.tab.h
krb5-parser.tab.o: krb5-lexer.lex.c krb5-parser.tab.h

krb5-parser.tab.c: krb5-parser.y
	${BISON} -o$@ -d krb5-parser.y

krb5-parser.tab.h: krb5-parser.tab.c

krb5-lexer.lex.c: krb5-lexer.l
	${LEX} ${LEXFLAGS} -o$@ krb5-lexer.l

test: ${PROG}
	${.CURDIR}/${PROG} -s domain_realm < /etc/krb5.conf

install:
	$(MKDIR) -p $(DESTDIR)
	$(INSTALL) -m $(BINMODE) $(PROG) $(DESTDIR)/


.include <bsd.prog.mk>
