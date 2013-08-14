#!/bin/sh
# Copyright 2007 PC-BSD Software
# http://www.pcbsd.com
# Author: Kris Moore
###########################################################################
MM_CHARSET="UTF-8" ; export MM_CHARSET
LANG="en_US.UTF-8" ; export LANG
LC_CTYPE="en_US.UTF-8" ; export LC_CTYPE     
LC_COLLATE="POSIX" ; export LC_COLLATE

# Make sure this is only executed on the first login, in order for other ctrl-fX screens to work
if [ -f "/tmp/1stRun" ] 
then
  exit
fi

touch /tmp/1stRun

# Make /var/tmp link
mv /var/tmp /tmp/vartmp
ln -s /tmp/vartmp /var/tmp

# Check ZFS tmpdir
if [ ! -d "/tmp/zfs" ] ; then
  mkdir /tmp/zfs
  if [ $? -ne 0 ] ; then
     echo "Error creating /tmp/zfs, ZFS installs will not work!"
     echo "Press ENTER to continue"
     read tmp
  fi
fi

# Check if we have FreeBSD dist files on this media
if [ ! -d "/dist-fbsd" ] ; then
   touch /tmp/no-fbsd-release
fi

# Enable the debug version of pc-sysinstall
/root/debugpcsysinstall.sh

# Set all NICS to DHCP
NICS=`ifconfig -l`
for i in $NICS
do
  ifconfig ${i} | grep -q "status: active"
  if [ $? -eq 0 ] ; then
    echo "Enabling networking on ${i}..."
    (dhclient ${i} >/dev/null 2>/dev/null ) &
  fi
done

# Check if we are booting in LIVE or INSTALL mode
if [ -e "/usr/pcbsd-live" ]; then

  # Check if we should be going to the console instead of X directly
  /sbin/sysctl kern.module_path | /usr/bin/grep "CONSOLE" >/dev/null 2>/dev/null
  FOUND="$?"
  if [ "$FOUND" = "0" ]
  then
    /root/PCBSDtext.sh
    shutdown -r now
    exit
  fi

  # We are running LIVE mode, start that script now
  sh /root/PCBSDStartLive.sh
  exit 0
fi

# Source our functions
. /root/functions.sh

# Check if we have an auto-install directive
if [ -e "/tmp/pc-autoinstall.conf" ]
then
  # Start the parser on this directive and begin the install
  /usr/sbin/pc-sysinstall start-autoinstall /tmp/pc-autoinstall.conf
fi

# Run the intial sorting of directories
mv /root/.fluxbox /memfs/
ln -s /memfs/.fluxbox /root/.fluxbox

# Create the .qt directory
mkdir /tmp/.qt
mkdir /tmp/xkb

# Check if we should be going to the console instead of X directly
/sbin/sysctl kern.module_path | /usr/bin/grep "CONSOLE" >/dev/null 2>/dev/null
FOUND="$?"
if [ "$FOUND" = "0" ]
then
  /root/PCBSDtext.sh
  shutdown -r now
  exit
fi

# Now run the X auto-detection
detect_x

# Now start xorg
start_xorg

# Check if we had a successful SysInstaller exit
if [ -e "/root/.exitStatus" ] ; then
  STATUS="`cat /root/.exitStatus`"
  if [ "$STATUS" = "0" ] ; then
    shutdown -r now
  else
    echo "Installation failed! Press any key to drop to shell or type 'reboot' to restart the system"
    read tmp
    if [ "$tmp" = "reboot" ] ; then
      shutdown -r now
    else
      /bin/sh
    fi
  fi
fi
