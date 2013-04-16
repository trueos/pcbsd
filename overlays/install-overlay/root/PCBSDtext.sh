#!/bin/sh
# PC-BSD System Install Menu
# Copyright 2007 PC-BSD Software
# http://www.pcbsd.com
# Author: Kris Moore
###########################################################################

DRES="1024x768" 

while
i="1"
do

dialog --title "PC-BSD Installation Menu" --menu "Please select from the following options:" 20 55 15 install "Start graphical install" vesa "Start graphical install (VESA)" utility "System Utilities" reboot "Reboot the system" 2>/tmp/answer

ANS="`cat /tmp/answer`"

case $ANS in
    install) echo "Starting PC-BSD System Installer.. Please wait.."
             startx 2>/tmp/Xerrors ;;
       vesa) echo "Starting PC-BSD System Installer.. Please wait.."
 cp /root/cardDetect/XF86Config.compat /etc/X11/xorg.conf
             startx 2>/tmp/Xerrors ;;
    utility) /root/PCBSDUtil.sh
              clear ;;
     reboot)  shutdown -r now ;;
          *) ;;
esac



done


