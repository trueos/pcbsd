PREFIX?= /usr/local
FILES=pc-activedirectory.conf pc-ldap.conf

FILESMODE=0400
FILESDIR=$(PREFIX)/etc
NO_OBJ=

install:
	$(MKDIR) -p $(FILESDIR)
	$(INSTALL) -m $(FILESMODE) $(FILES) $(FILESDIR)/

.include <bsd.prog.mk>
