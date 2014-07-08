#!/bin/sh

if [ "`id -u`" = "0" ] ; then return ; fi

#Startup the mixer tray
(sleep 30 ; pc-mixer ) &

