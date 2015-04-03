PREFIX?= /usr/local
FILES=rc.activedirectory rc.ldap
MKDIR=		mkdir

FILESMODE=0444
FILESDIR=$(PREFIX)/etc
NO_OBJ=

install:
	$(MKDIR) -p $(FILESDIR)
	$(INSTALL) -m $(FILESMODE) $(FILES) $(FILESDIR)/

.include <bsd.prog.mk>
