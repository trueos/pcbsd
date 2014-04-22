#!/bin/sh

sed -i.bak "s,exec,LD_PRELOAD='/compat/linux/usr/lib/libGL.so.1' ; export LD_PRELOAD ; exec,g" ${PBI_PROGDIRPATH}/bin/linux-quake4
sed -i.bak "s,exec,LD_PRELOAD='/compat/linux/usr/lib/libGL.so.1' ; export LD_PRELOAD ; exec,g" ${PBI_PROGDIRPATH}/bin/linux-quake4smp
sed -i.bak "s,exec,LD_PRELOAD='/compat/linux/usr/lib/libGL.so.1' ; export LD_PRELOAD ; exec,g" ${PBI_PROGDIRPATH}/bin/linux-quake4ded

chmod 755 ${PBI_PROGDIRPATH}/bin/*
