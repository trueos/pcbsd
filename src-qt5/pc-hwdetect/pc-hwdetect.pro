TARGET = pc-hwdetect
target.path=/usr/local/bin/
TEMPLATE = app
LIBS +=-L/usr/local/lib -lxml2  -L/usr/local/lib -lcurl -lutil
INCLUDEPATH += /usr/local/include /usr/local/include/libxml2 /usr/local/include/curl
SOURCES += acpi_probe.c hwdetect.c pciconf.c xmlparse.c

chmod.path=/usr/local/etc/rc.d
chmod.extra=cd rc.d ; chmod 755 pc-hwdetect

rcd.path=/usr/local/etc/rc.d/
rcd.extra=cp rc.d/* $(INSTALL_ROOT)/usr/local/etc/rc.d/

xml.path=/usr/local/share/pcbsd/pc-hwdetect/
xml.files=xml/*

INSTALLS += target chmod rcd xml
