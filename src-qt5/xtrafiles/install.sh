#!/bin/sh

DEFAULT="/usr/local"

if [ -z "$1" ] ; then
        LB="${DEFAULT}"
else
        LB="${1}"
fi

echo "Installing extra files to ${LB}"

#Make sure to clean up certain directories before applying the overlay
if [ -e ${LB}/share/pcbsd/xstartup ]; then
  echo " - Cleaning ${LB}/share/pcbsd/xstartup first"
  rm ${LB}/share/pcbsd/xstartup/*.sh
fi
if [ -e ${LB}/etc/xdg/autostart ]; then
  echo " - Cleaning ${LB}/etc/xdg/autostart/pcbsd-* entries"
  rm ${LB}/etc/xdg/autostart/pcbsd-*.desktop
fi

# Copy files to system
tar cvf - -C local . 2>/dev/null | tar xvf - -C ${LB} 2>/dev/null
if [ $? -ne 0 ] ; then
  exit 1
fi

cd local
for i in `find .`
do
   chown root:wheel ${LB}/${i}
done
cd ..

exit 0
