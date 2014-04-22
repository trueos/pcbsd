#!/bin/sh

cd /usr/ports/print/freetype2
make install clean

cd /usr/ports/sysutils/hal
make clean
make
make install

cd /usr/ports/graphics/lcms
make install clean

cd /usr/ports/print/cups
make install clean

cd /usr/ports/graphics/png
make install clean

cd /usr/ports/graphics/jpeg
make install clean
