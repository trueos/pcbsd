#!/bin/sh

# Enable the various required services
grep '^mysql_enable="YES"' /etc/rc.conf >/dev/null 2>/dev/null
if [ $? -ne 0 ] ; then
	echo 'mysql_enable="YES"' >>/etc/rc.conf
fi
grep '^mythbackend_enable="YES"' /etc/rc.conf >/dev/null 2>/dev/null
if [ $? -ne 0 ] ; then
	echo 'mythbackend_enable="YES"' >>/etc/rc.conf
fi
