#!/bin/sh


ln -s ${PBI_PROGDIRPATH}/modules/kqemu.ko /boot/modules/kqemu.ko
kldload /boot/modules/kqemu.ko
kldload /boot/kernel/aio.ko

cp /boot/loader.conf /boot/loader.conf.orig
cat /boot/loader.conf | grep -v "kqemu_load" | grep -v "aio_load" > /tmp/loader.conf
echo 'kqemu_load="YES"' >> /tmp/loader.conf
echo 'aio_load="YES"' >> /tmp/loader.conf
mv /tmp/loader.conf /boot/loader.conf


