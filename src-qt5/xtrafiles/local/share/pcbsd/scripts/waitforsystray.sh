#!/bin/sh
#This is a simple script to wait until a system tray is registered before continuing on

ready="FALSE"
while [ ${ready} == "FALSE" ]
do
  #The system tray is registered with the _NET_SYSTEM_TRAY_S1 atom, and *should* be a child of the root window
  #First get all the window ID's
  for i in `xwininfo -root -tree | grep + | grep 0x`
  do
    #This goes word by word (at the moment)
    #Pull the ID out of this line (first word is the Hex ID)
    #echo "Check Line: ${i}"
    #WID=`echo "${i}" | cut -w -f 1,1`
    WID="${i}"
    TMP=`echo "${WID}" | grep 0x | grep -v +`
    if [ "x${TMP}" == "x" ]; then
      continue #invalid WID
    fi
    echo "Found WID: $WID"
    #Now scan that window for the atom
    TMP=`xprop -id $WID | grep "_NET_SYSTEM_TRAY_"`
    #echo "INFO: ${TMP}"
    if [ ! "x" == "x${TMP}" ]; then
      echo "Found System Tray";
      ready="TRUE"
      break
    fi
  done
  sleep 15
done
