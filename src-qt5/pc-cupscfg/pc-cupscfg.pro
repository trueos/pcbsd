cupscfg.path=/usr/local/bin/
cupscfg.extra=cp pc-cupscfg $(INSTALL_ROOT)/usr/local/bin/

cupsmod.path=/usr/local/bin/
cupsmod.extra=chmod 755 $(INSTALL_ROOT)/usr/local/bin/pc-cupscfg

INSTALLS += cupscfg cupsmod
