#!/bin/sh

sed -i.bak "s,export,LD_PRELOAD='/compat/linux/usr/lib/libGL.so.1' ; export LD_PRELOAD,g" ${PBI_PROGDIRPATH}/lib/americasarmy/armyops
chmod 755 ${PBI_PROGDIRPATH}/bin/*
