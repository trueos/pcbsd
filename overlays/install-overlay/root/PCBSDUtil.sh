#!/bin/sh
# PC-BSD Install CD Utility Menu
# Copyright 2006 PC-BSD Software
# http://www.pcbsd.com
# Author: Kris Moore
############################################################################

ECHO="/bin/echo" ; export ECHO

# Source our functions
. /root/functions.sh

while
i="1"
do

# Display Utility Menu
dialog --title "PC-BSD Utility Menu" --menu "Please select from the following options:" 20 55 15 shell "Drop to emergency shell" zimport "Import / mount zpool" fixgrub "Restamp GRUB on disk" exit "Exit Utilities" 2>/tmp/UtilAnswer

ANS="`cat /tmp/UtilAnswer`"

case $ANS in
      shell) clear ; echo "# PC-BSD Emergency Shell
#
# Please type 'exit' to return to the menu
#############################################################"

              /bin/csh ;;
     zimport) zpool_import ;;
     fixgrub) restamp_grub_install ;;
        exit) break ; exit 0 ;;
          *) ;;
esac

done

