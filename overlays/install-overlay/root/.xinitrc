#!/bin/sh

# Touch our marker, so that users can login on other ttys
touch /tmp/.xstarted

# Turn off the numlock
numlockx off

# Run fluxbox in the background
/usr/local/bin/startfluxbox &

# Make sure we have our libs
LD_LIBRARY_PATH="/usr/local/lib/qt4" ; export LD_LIBRARY_PATH

# Turn off screensaver / blanking
xset s off
xset -dpms

# Start the installer
/usr/local/bin/pc-sysinstaller 2>/tmp/.SysInstall.log
echo "$?" > /root/.exitStatus
