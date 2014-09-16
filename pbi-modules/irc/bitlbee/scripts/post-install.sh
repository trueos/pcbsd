#!/bin/sh

# Check if we have a bitlbee.conf file, copy example if not
if [ ! -e "/usr/local/etc/bitlbee/bitlbee.conf" ] ; then
   cp /usr/local/share/examples/bitlbee/bitlbee.conf /usr/local/etc/bitlbee/bitlbee.conf
   chmod 755 /usr/local/etc/bitlbee/bitlbee.conf
   cp /usr/local/share/examples/bitlbee/motd.txt /usr/local/etc/bitlbee/motd.txt
   chmod 755 /usr/local/etc/bitlbee/motd.txt

   # Now to set the defaults we want
   sed -i '' 's|# RunMode = Inetd|RunMode = ForkDaemon|g' /usr/local/etc/bitlbee/bitlbee.conf
   sed -i '' 's|# User = bitlbee|User = bitlbee|g' /usr/local/etc/bitlbee/bitlbee.conf
   sed -i '' 's|# DaemonInterface = 0.0.0.0|DaemonInterface = 0.0.0.0|g' /usr/local/etc/bitlbee/bitlbee.conf
   sed -i '' 's|# DaemonPort = 6667|DaemonPort = 6667|g' /usr/local/etc/bitlbee/bitlbee.conf
   sed -i '' 's|# AuthMode = Open|AuthMode = Open|g' /usr/local/etc/bitlbee/bitlbee.conf
   sed -i '' 's|# OperPassword = ChangeMe!|OperPassword = ChangeMe!|g' /usr/local/etc/bitlbee/bitlbee.conf
   sed -i '' 's|# MotdFile = /usr/local/etc/bitlbee/motd.txt|MotdFile = /usr/local/etc/bitlbee/motd.txt|g' /usr/local/etc/bitlbee/bitlbee.conf
   sed -i '' 's|# ConfigDir = /var/db/bitlbee|ConfigDir = /var/db/bitlbee|g' /usr/local/etc/bitlbee/bitlbee.conf
fi
