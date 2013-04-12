#!/bin/sh

# Enable loading the nvidia binary driver
grep '^nvidia_load="YES"' /boot/loader.conf >/dev/null 2>/dev/null
if [ $? -eq 0 ] ; then
	sed -i .bak '/nvidia_load="YES"/d' /boot/loader.conf
fi
