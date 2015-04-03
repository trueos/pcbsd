PREFIX?= /usr/local
MKDIR=		mkdir

FILES=pc-adctl pc-ldapctl

FILESMODE=0555
FILESDIR=$(PREFIX)/bin
NO_OBJ=

install:
	$(MKDIR) -p $(FILESDIR)
	$(INSTALL) -m $(FILESMODE) $(FILES) $(FILESDIR)/

.include <bsd.prog.mk>
