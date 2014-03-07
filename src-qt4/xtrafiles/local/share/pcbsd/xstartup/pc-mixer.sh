#!/bin/sh

if [ "`id -u`" = "0" ] ; then return ; fi

#Disable this from autostart for now until after further testing
return

#Startup the mixer tray
(sleep 20 ; sudo pc-mixer ) &

