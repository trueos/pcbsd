#!/bin/sh
# Script to check if there are any wireless networks to display on the tray

if [ "`id -u`" = "0" ] ; then return ; fi

(sleep 30 ; ifconfig wlan0 && sudo pc-nettray wlan0 ) &

(sleep 35 ; ifconfig wlan1 && sudo pc-nettray wlan1 ) &

