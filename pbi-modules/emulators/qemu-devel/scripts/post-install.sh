#!/bin/sh


kldload /boot/kernel/aio.ko

cp /boot/loader.conf /boot/loader.conf.orig
cat /boot/loader.conf | grep -v "aio_load" > /tmp/loader.conf
echo 'aio_load="YES"' >> /tmp/loader.conf
mv /tmp/loader.conf /boot/loader.conf


