#!/bin/sh

if [ "`id -u`" = "0" ] ; then return ; fi

if [ ! -z "`dmesg | grep ubt0`" ]; then
      (sleep 10; sudo pc-bluetoothtray) &
fi

