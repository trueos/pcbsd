#!/bin/sh
# Start life-preserver with login

if [ "`id -u`" = "0" ] ; then return ; fi

(sleep 30 ; sudo life-preserver -tray -autostart) &
