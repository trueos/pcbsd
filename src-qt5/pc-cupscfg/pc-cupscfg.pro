cupscfg.path=/usr/local/bin/
cupscfg.files=pc-cupscfg

cupsmod.path=/usr/local/bin/
cupsmod.extra=chmod 755 $(INSTALL_ROOT)/usr/local/bin/pc-cupscfg

INSTALLS += cupscfg cupsmod
