#!/bin/sh

# Enable loading the vboxguest binary driver
grep '^vboxguest_enable="YES"' /etc/rc.conf >/dev/null 2>/dev/null
if [ $? -ne 0 ] ; then
	echo 'vboxguest_enable="YES"' >>/etc/rc.conf
fi
grep '^vboxservice_enable="YES"' /etc/rc.conf >/dev/null 2>/dev/null
if [ $? -ne 0 ] ; then
	echo 'vboxservice_enable="YES"' >>/etc/rc.conf
fi
grep '^vboxguest_load="YES"' /boot/loader.conf >/dev/null 2>/dev/null
if [ $? -ne 0 ] ; then
	echo 'vboxguest_load="YES"' >>/boot/loader.conf
fi
