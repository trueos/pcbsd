#!/bin/sh

# Source config file
. ../config.sh

DEFAULT="/usr/local"

if [ -z "$1" ] ; then
        LB="${DEFAULT}"
else
        LB="${1}"
fi

cp pc-extractoverlay ${LB}/bin/pc-extractoverlay
if [ $? -ne 0 ] ; then
  exit 1
fi
chmod 755 ${LB}/bin/pc-extractoverlay
if [ $? -ne 0 ] ; then
  exit 1
fi

if [ ! -d "${LB}/share/pcbsd/conf" ] ; then
  mkdir -p ${LB}/share/pcbsd/conf
fi
if [ ! -d "${LB}/share/pcbsd/distfiles" ] ; then
  mkdir -p ${LB}/share/pcbsd/distfiles
fi

# Copy exclude lists
cp desktop-excludes ${LB}/share/pcbsd/conf
if [ $? -ne 0 ] ; then
  exit 1
fi
cp server-excludes ${LB}/share/pcbsd/conf
if [ $? -ne 0 ] ; then
  exit 1
fi

# Set the default package set
sed -i '' "s|PACKAGE_SET:.*|PACKAGE_SET: $INSTALLPACKAGESET|g" server-overlay/usr/local/etc/pcbsd.conf
sed -i '' "s|PACKAGE_SET:.*|PACKAGE_SET: $INSTALLPACKAGESET|g" desktop-overlay/usr/local/etc/pcbsd.conf

# Now create desktop-overlay.txz file
tar cvJ --uname "root" --gname "wheel" -f ${LB}/share/pcbsd/distfiles/desktop-overlay.txz -C desktop-overlay .
if [ $? -ne 0 ] ; then
  exit 1
fi

# Now create server-overlay.txz file
tar cvJ --uname "root" --gname "wheel" -f ${LB}/share/pcbsd/distfiles/server-overlay.txz -C server-overlay .
if [ $? -ne 0 ] ; then
  exit 1
fi

exit 0
