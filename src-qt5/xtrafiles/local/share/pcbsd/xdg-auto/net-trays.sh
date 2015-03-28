#!/bin/sh
# Script to check if there are any wireless networks to display on the tray

if [ "`id -u`" = "0" ] ; then return ; fi

ifconfig wlan0 >/dev/null 2>/dev/null
if [ $? -eq 0 ] ; then
  sudo pc-nettray wlan0
fi

ifconfig wlan1 >/dev/null 2>/dev/null
if [ $? -eq 0 ] ; then
  sudo pc-nettray wlan1
fi
