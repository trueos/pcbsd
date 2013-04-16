#!/bin/sh

# Remove unnecessary bits
grep '^mythbackend_enable="YES"' /etc/rc.conf >/dev/null 2>/dev/null
if [ $? -eq 0 ] ; then
	sed -i .bak '/mythbackend_enable="YES"/d' /etc/rc.conf
fi
