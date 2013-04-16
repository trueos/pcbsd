#!/bin/sh

# Check if we need to start the screensaver on GNOME
echo $DESKTOP_SESSION | grep -q -e "gnome"
if [ $? -eq 0 ] ; then
  xscreensaver -no-splash &
fi

