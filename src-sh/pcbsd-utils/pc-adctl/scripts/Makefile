PREFIX?= /usr/local
FILES=pc-activedirectory pc-kerberos pc-kinit pc-nsswitch pc-pam \
	pc-samba pc-ldap pc-nssldap pc-directory

MKDIR=mkdir
FILESMODE=0555
FILESDIR=$(PREFIX)/etc/rc.d
NO_OBJ=

install:
	$(MKDIR) -p $(FILESDIR)
	$(INSTALL) -m $(FILESMODE) $(FILES) $(FILESDIR)/

.include <bsd.prog.mk>
