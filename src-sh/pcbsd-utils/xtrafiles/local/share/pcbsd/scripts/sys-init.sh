#!/bin/sh
# Script which does some first time init

case $1 in
 desktop|server) ;;
              *) echo "Error: Please specify 'desktop' or 'server'"
                 exit 1
                 ;;
esac

################################################
# Do generic init
################################################

if [ ! -d "/usr/home" ] ; then
   mkdir /usr/home
fi

# Setup /home link
if [ ! -e "/home" ] ; then
  ln -s /usr/home /home
fi

# Init the firewall
sh /usr/local/share/pcbsd/scripts/reset-firewall

# Update XDG menus
/usr/local/bin/pc-xdgutil updatemenu

# Add our notice to top of loader.conf
if [ -e "/boot/loader.conf" ] ; then
  mv /boot/loader.conf /boot/loader.conf.tmp
fi

cat <<EOF >/boot/loader.conf
# * IMPORTANT NOTICE *
# If using GRUB, run 'grub-mkconfig -o /boot/grub/grub.cfg' after making
# changes to this file
###############################################################################
EOF

if [ -e "/boot/loader.conf.tmp" ] ; then
  cat /boot/loader.conf.tmp >> /boot/loader.conf
  rm /boot/loader.conf.tmp
fi

# Tune ZFS ARC for desktop / server usage
###############################################

# Get system memory in bytes
sysMem="`sysctl hw.physmem | cut -d ' ' -f 2`"
# Get that in MB
sysMem=`expr $sysMem / 1024 / 1024`

# Set some default zArc sizes based upon RAM of system
if [ $sysMem -lt 1024 ] ; then
  zArc="128"
elif [ $sysMem -lt 2048 ] ; then
  zArc="256"
elif [ $sysMem -lt 4096 ] ; then
  zArc="512"
else
  zArc="1024"
fi

# If we don't already have an ARC setting, save it
grep -q "vfs.zfs.arc_max" /boot/loader.conf
if [ $? -ne 0 ] ; then
  echo "# Tune ZFS Arc Size - Change to adjust memory used for disk cache" >> /boot/loader.conf
  echo "vfs.zfs.arc_max=\"${zArc}M\"" >> /boot/loader.conf
fi

################################################
# Do desktop specific init
################################################
if [ "$1" = "desktop" ] ;then

  # Set for desktop mode
  touch /etc/defaults/trueos-desktop
  chflags schg /etc/defaults/trueos-desktop

  # Copy the default desktop files over
  echo "Copying defaults to base system"
  tar cvf - -C /usr/local/share/pcbsd/desktop-defaults . 2>/dev/null | tar xvf - -C / 2>/dev/null

  # Need to save a language?
  if [ -n "$2" ] ; then
     echo "$2" > /etc/pcbsd-lang
  fi
fi

################################################
# Do server specific init
################################################
if [ "$1" = "server" ] ; then
  # Set running a server
  touch /etc/defaults/trueos-server
  chflags schg /etc/defaults/trueos-server

  # Copy the default server files over
  echo "Copying defaults to base system"
  tar cvf - -C /usr/local/share/pcbsd/server-defaults . 2>/dev/null | tar xvf - -C / 2>/dev/null
fi

# Boot-strap the PKG config
pc-updatemanager syncconf

################################################
# Specific setup if installing into pre-built VM
################################################
if [ "$3" = "vm" ] ; then
   # Since the NIC may change, set all to DHCP
   echo "ifconfig_DEFAULT=\"DHCP\"" >> /etc/rc.conf
fi
