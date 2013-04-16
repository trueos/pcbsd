#!/bin/sh
# PC-BSD Install CD Utility Menu
# Copyright 2006 PC-BSD Software
# http://www.pcbsd.com
# Author: Kris Moore
############################################################################

ECHO="/bin/echo" ; export ECHO

while
i="1"
do

# Display Utility Menu
dialog --title "PC-BSD Utility Menu" --menu "Please select from the following options:" 20 55 15 fdisk "Manually run fdisk" shell "Drop to emergency shell" chroot "Chroot into root partition" exit "Exit Utilities" 2>/tmp/UtilAnswer

ANS="`cat /tmp/UtilAnswer`"

case $ANS in
      fdisk) clear 
		MM_CHARSET="" ; export MM_CHARSET
		LANG="" ; export LANG
		LC_COLLATE="" ; export LC_COLLATE

             dialog --title "Manual Fdisk" --msgbox "Here you can manually partition your drive in preparation for PC-BSD. When you are finished, please select the option to start the graphical installation." 10 40
             /usr/sbin/sysinstall diskPartitionEditor 
		MM_CHARSET="UTF-8" ; export MM_CHARSET
		LANG="en_US.UTF-8" ; export LANG
		LC_COLLATE="POSIX" ; export LC_COLLATE
             clear ;;
      shell) clear ; echo "# PC-BSD Emergency Shell
#
# Please type 'exit' to return to the menu
#############################################################"

              /bin/csh ;;
     chroot) clear 
             $ECHO "Enter the root partition of the drive you wish
to chroot to. (Example: /dev/ad0s1a for an IDE drive.)
Select Partition:\c"
             read partition
             if [ -e "$partition" ]
             then
                 mount $partition /mnt
                 if [ "$?" = "0" ]
                 then
                     $ECHO "# PC-BSD chroot session
#
# Please type 'exit' to return to the menu
#############################################################"
                     chroot /mnt /bin/sh
                     umount /mnt
                     $ECHO "(Press Enter to continue)\c" ; read tmp
                 else 
                    $ECHO "Error mounting partition ($partition) (Press Enter to continue)\c" ; read tmp
                 fi
             else
                $ECHO "Device doesn't exist! (Press Enter to continue)\c"; read tmp
             fi

             ;;
        exit) break ; exit 0 ;;
          *) ;;
esac

done

