#!/bin/sh
# Start the virtual keyboard when running with a touch-screen

# See if we have a touch-screen auto-configured
cat /etc/X11/xorg.conf | grep -q "Panel0" >/dev/null 2>/dev/null
if [ $? -ne 0 ] ; then return 0 ; fi

if [ ! -e "$HOME/.florence.conf" ] ; then
  cp /usr/local/share/florence/florence.conf ${HOME}/.florence.conf   
fi

(/usr/local/libexec/at-spi-registryd) &
(sleep 45 ; /usr/local/bin/florence --use-config ${HOME}/.florence.conf --no-gnome ) &

