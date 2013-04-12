#!/bin/sh

# Enable loading the nvidia binary driver
grep '^nvidia_load="YES"' /boot/loader.conf >/dev/null 2>/dev/null
if [ $? -ne 0 ] ; then
	echo 'nvidia_load="YES"' >>/boot/loader.conf
fi
