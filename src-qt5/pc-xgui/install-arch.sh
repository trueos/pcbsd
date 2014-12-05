#!/bin/sh

tDIR="/usr/local/share/pcbsd/xorg-gui/settings"
lDIR="$1"

arch=`uname -m`
cp ${lDIR}/resources/settings/drivers.txt.${arch} ${tDIR}/drivers.txt
