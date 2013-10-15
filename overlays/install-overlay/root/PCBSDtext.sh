#!/bin/sh
# PC-BSD System Install Menu
# Copyright 2013 PC-BSD Software
# http://www.pcbsd.com
# Author: Kris Moore
###########################################################################

. /root/functions.sh

text_install_menu()
{

  while :
  do
    if [ -e "/tmp/sys-install.cfg" ] ; then
      dialog --title "PC-BSD Text Install" --menu "Please select from the following options:" 20 55 15 wizard "Re-Run install wizard" edit "Edit install script" install "Start installation" back "Return to previous menu" 2>/tmp/answer
    else
      dialog --title "PC-BSD Text Install" --menu "Please select from the following options:" 20 55 15 wizard "Run install wizard" back "Return to previous menu" 2>/tmp/answer
    fi

    ANS="`cat /tmp/answer`"

    case $ANS in
     wizard) pc-installdialog
             rtn
             ;;
       edit) vi /tmp/sys-install.cfg
             rtn
             ;;
    install) echo "This will begin the installation, continue?"
             echo -e "(y/n)\c"
             read tmp
             if [ "$tmp" = "y" -o "$tmp" = "Y" ] ; then
                pc-sysinstall -c /tmp/sys-install.cfg
                rtn
             fi
             ;;
       back) break ;;
          *) ;;
    esac
  done
}

DRES="1024x768" 

while
i="1"
do

dialog --title "PC-BSD Installation Menu" --menu "Please select from the following options:" 20 55 15 install "Start text install" xorg "Start graphical install" vesa "Start graphical install (VESA)" utility "System Utilities" reboot "Reboot the system" 2>/tmp/answer

ANS="`cat /tmp/answer`"

case $ANS in
    install) text_install_menu ;;
    xinstall) echo "Starting PC-BSD System Installer.. Please wait.."
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


