#!/bin/sh

pkg info | grep -q nvidia-driver
if [ $? -ne 0 ] ; then
	exit 0
fi

# Run the wine nvidia setup
sh /usr/local/share/wine/patch-nvidia.sh
