TARGET = pc-sounddetect
target.path=/usr/local/bin/
TEMPLATE = app
LIBS +=-L/usr/local/lib -lxml2  -L/usr/local/lib -lcurl -lutil
INCLUDEPATH += /usr/local/include /usr/local/include/libxml2 /usr/local/include/curl
SOURCES += parse_sound.c sound_detect.c

chmod.path=/usr/local/etc/rc.d
chmod.extra=cd rc.d ; chmod 755 pc-sounddetect

rcd.path=/usr/local/etc/rc.d/
rcd.files=rc.d/*

xml.path=/usr/local/share/pcbsd/pc-sounddetect/
xml.files=xml/*

INSTALLS += target chmod rcd xml
