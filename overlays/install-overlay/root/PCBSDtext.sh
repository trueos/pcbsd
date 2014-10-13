#!/bin/sh
# PC-BSD System Install Menu
# Copyright 2013 PC-BSD Software
# http://www.pcbsd.com
# Author: Kris Moore
###########################################################################

. /root/functions.sh

DRES="1024x768" 

while
i="1"
do

if [ -e "/usr/local/bin/startx" ] ; then
  dialog --title "PC-BSD Installation Menu" --menu "Please select from the following options:" 20 55 15 install "Start text install" xorg "Start graphical install" vesa "Start graphical install (VESA)" utility "System Utilities" reboot "Reboot the system" 2>/tmp/answer
else
  dialog --title "PC-BSD Installation Menu" --menu "Please select from the following options:" 20 55 15 install "Start text install" utility "System Utilities" reboot "Reboot the system" 2>/tmp/answer
fi

ANS="`cat /tmp/answer`"

case $ANS in
    install) /usr/local/bin/pc-installdialog ;;
       xorg) echo "Starting PC-BSD System Installer.. Please wait.."
             startx 2>/tmp/Xerrors ;;
       vesa) echo "Starting PC-BSD System Installer.. Please wait.."
 cp /root/cardDetect/XF86Config.compat /etc/X11/xorg.conf
             startx 2>/tmp/Xerrors ;;
    utility) /root/PCBSDUtil.sh
              clear ;;
     reboot)  reboot -q ;;
          *) ;;
esac



done


