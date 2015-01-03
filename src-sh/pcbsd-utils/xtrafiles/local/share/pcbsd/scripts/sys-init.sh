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

# Update ports overlay files
/usr/local/bin/pc-extractoverlay ports

# Add our notice to top of loader.conf
if [ -e "/boot/loader.conf" ] ; then
  mv /boot/loader.conf /boot/loader.conf.tmp
fi

cat <<EOF >/boot/loader.conf
# * IMPORTANT NOTICE *
# Run 'grub-mkconfig -o /boot/grub/grub.cfg' after making changes to this file
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

  # Init the flash plugin for all users
  cd /home
  for i in `ls -d * 2>/dev/null`
  do
    su ${i} -c "flashpluginctl off"
    su ${i} -c "flashpluginctl on"
  done

  # Enable the system updater tray
  pbreg set /PC-BSD/SystemUpdater/runAtStartup true

  # Set running desktop
  pbreg set /PC-BSD/SysType PCBSD
  touch /etc/defaults/pcbsd
  chflags schg /etc/defaults/pcbsd

  # Init the desktop
  /usr/local/bin/pc-extractoverlay desktop --sysinit

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
  pbreg set /PC-BSD/SysType TRUEOS
  touch /etc/defaults/trueos
  chflags schg /etc/defaults/trueos

  # Init the server
  /usr/local/bin/pc-extractoverlay server --sysinit
fi

################################################
# Specific setup if installing into pre-built VM
################################################
if [ "$3" = "vm" ] ; then
   # Since the NIC may change, set all to DHCP
   echo "ifconfig_DEFAULT=\"DHCP\"" >> /etc/rc.conf
fi

################################################
# Do we have AppCafe remote files to process?
################################################

if [ -e "/tmp/appcafe-user" -a -e "/tmp/appcafe-pass" ] ; then
  appUser="`cat /tmp/appcafe-user`"

  # Set the AppCafe username / password now
  cat /tmp/appcafe-pass | /usr/local/bin/appcafe-setpass $appUser --

  # Remove temp files
  rm /tmp/appcafe-user
  rm /tmp/appcafe-pass

  # Check if the conf file is ready
  if [ ! -e "/usr/local/etc/appcafe.conf" ] ; then
     cp /usr/local/etc/appcafe.conf.dist /usr/local/etc/appcafe.conf
  fi

  if [ -e "/tmp/appcafe-port" ] ; then
     appPort="`cat /tmp/appcafe-port`"

     # Set the port now
     sed -i '' "s|port = 8885|port = $appPort|g" /usr/local/etc/appcafe.conf
  fi

  # Enable remote access now
  sed -i '' 's|remote = false|remote = true|g' /usr/local/etc/appcafe.conf
fi
