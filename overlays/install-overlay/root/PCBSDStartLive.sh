#!/bin/sh
# Script which sets up our LIVE stuff

clear

# Source our functions
. /root/functions.sh

PATH="${PATH}:/usr/local/bin"
export PATH

# Start some services
/usr/local/etc/rc.d/pc-sounddetect start

# Remove old .xinitrc
rm /root/.xinitrc

# Prompt for which desktop to run
dialog --title "Select Desktop" --menu "Please select the desktop to load:" 12 30 15 KDE "KDE" GNOME "GNOME" LXDE "LXDE" XFCE "XFCE" 2>/tmp/UtilAnswer

ANS="`cat /tmp/UtilAnswer`"

case $ANS in
       KDE) dCmd="startkde4";;
     GNOME) dCmd="gnome-session";;
      LXDE) dCmd="startlxde";;
      XFCE) dCmd="startxfce4";;
         *) dCmd="startlxde";;
esac

# Setup our script to start X right away
echo "touch /tmp/.xstarted

# Set the default cd mount point so we can install from live media
CDMNT=/cdmnt
export CDMNT

# Turn off the numlock
numlockx off

/usr/local/bin/${dCmd}" >/root/.xinitrc

# Now run the X auto-detection
detect_x

# Now start xorg
start_xorg

# Reboot since we are finished
echo "Live mode finished! Enter 'exit' to reboot."
/bin/sh

shutdown -r now
