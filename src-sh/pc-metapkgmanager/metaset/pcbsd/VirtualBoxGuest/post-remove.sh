#!/bin/sh

# Enable loading the vboxguest binary driver
grep '^vboxguest_enable="YES"' /etc/rc.conf >/dev/null 2>/dev/null
if [ $? -eq 0 ] ; then
	sed -i .bak '/vboxguest_enable="YES"/d' /etc/rc.conf
fi
grep '^vboxservice_enable="YES"' /etc/rc.conf >/dev/null 2>/dev/null
if [ $? -eq 0 ] ; then
	sed -i .bak '/vboxservice_enable="YES"/d' /etc/rc.conf
fi
grep '^vboxguest_load="YES"' /boot/loader.conf >/dev/null 2>/dev/null
if [ $? -eq 0 ] ; then
	sed -i .bak '/vboxguest_load="YES"/d' /boot/loader.conf
fi
