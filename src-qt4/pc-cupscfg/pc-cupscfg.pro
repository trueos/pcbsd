cupscfg.path=/usr/local/bin/
cupscfg.files=pc-cupscfg

cupsmod.path=/usr/local/bin/
cupsmod.extra=chmod 755 /usr/local/bin/pc-cupscfg

INSTALLS += cupscfg cupsmod
