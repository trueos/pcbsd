#!/bin/sh

sed -i.bak "s,exec,LD_PRELOAD='/compat/linux/usr/lib/libGL.so.1' ; export LD_PRELOAD ; exec,g" ${PBI_PROGDIRPATH}/bin/ut2004-demo
chmod 755 ${PBI_PROGDIRPATH}/bin/*
