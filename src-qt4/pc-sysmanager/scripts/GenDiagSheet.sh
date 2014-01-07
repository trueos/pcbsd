#!/bin/sh

PCVER="`uname -r`"
DATE="`date`"

echo "PC-BSD $PCVER Diagnostic Sheet
Generated: $DATE

---------------PC-BSD Packages------------------------

`pkg info 'pcbsd-*'`

-----------------Dmesg Output-------------------------

`dmesg`

-----------------Messages Output----------------------

`tail -20 /var/log/messages`

------------------pciconf-----------------------------

`pciconf -lv`

------------------xorg.conf---------------------------

`cat /etc/X11/xorg.conf`

------------------RC File-----------------------------

`cat /etc/rc.conf`

------------------Loader File-------------------------

`cat /boot/loader.conf`

------------------Free Space--------------------------

`df -m`

------------------System Processes--------------------

`top -n`



" > ${1}

chown ${2}:${2} ${1}
