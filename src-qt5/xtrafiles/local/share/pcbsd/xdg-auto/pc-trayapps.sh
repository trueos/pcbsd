#!/bin/sh

#None of these tray apps should be started as root
if [ "`id -u`" = "0" ]
then return ; 
fi

#Wait until a system tray is available
/usr/local/share/pcbsd/scripts/waitforsystray.sh
#Now startup the tray apps
if [ ! ${XDG_CURRENT_DESKTOP} == "LUMINA" ]; then
  (pc-mixer >/dev/null) &
fi
(pc-mounttray >/dev/null) &
(life-preserver-tray >/dev/null) &
(pc-systemupdatertray >/dev/null) &

ifconfig wlan0 >/dev/null 2>/dev/null
if [ $? -eq 0 ] ; then
	(sudo pc-nettray wlan0 >/dev/null) &
fi

ifconfig wlan1 >/dev/null 2>/dev/null
if [ $? -eq 0 ] ; then
	(sudo pc-nettray wlan1 >/dev/null) &
fi
