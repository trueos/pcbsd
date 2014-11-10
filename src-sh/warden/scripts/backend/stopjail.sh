#!/bin/sh 
# Script to stop a jail
# Args $1 = jail-name
#######################################################################

# Source our functions
PROGDIR="/usr/local/share/warden"

# Source our variables
. ${PROGDIR}/scripts/backend/functions.sh

JAILNAME="${1}"
if [ "${2}" = "FAST" ]
then
  FAST="Y"
fi

if [ -z "${JAILNAME}" ]
then
  echo "ERROR: No jail specified to delete!"
  exit 5
fi

if [ -z "${JDIR}" ]
then
  echo "ERROR: JDIR is unset!!!!"
  exit 5
fi

JAILDIR="${JDIR}/${JAILNAME}"

if [ ! -d "${JAILDIR}" ]
then
  echo "ERROR: No jail located at ${JAILDIR}"
  exit 5
fi

HOST="`cat ${JMETADIR}/host`"

# Check if we need to enable vnet
VIMAGEENABLE="NO"
if [ -e "${JMETADIR}/vnet" ] ; then
  VIMAGEENABLE="YES"
fi

IFACE=
DEFAULT=0
  
# Make sure jail uses special interface if specified
if [ -e "${JMETADIR}/iface" ] ; then
  IFACE=`cat "${JMETADIR}/iface"`
fi
if [ -z "${IFACE}" ] ; then
  if [ -n "$NIC" ] ; then
    IFACE="$NIC"
  else
    IFACE=`get_default_interface`
    DEFAULT=1
  fi
fi
if [ -z "${IFACE}" ] ; then
  echo "ERROR: no interface specified and a default doesn't exist!"
  exit 6
fi

# End of error checking, now shutdown this jail
##################################################################

echo -e "Stopping the jail...\c"

# Get the JailID for this jail
JID="`jls | grep ${JAILDIR}$ | tr -s " " | cut -d " " -f 2`"

echo -e ".\c"

jFlags=""
# Grab any additional jail flags
if [ -e "${JMETADIR}/jail-flags" ] ; then
  jFlags=`cat ${JMETADIR}/jail-flags`
fi

# If the user has enabled mounting of ZFS dataset, lets un-export this dataset to the jail
echo $jFlags | grep -q "allow.mount.zfs=1"
if [ $? -eq 0 ] ; then
   # Run the ZFS command to export the dataset
   jDataSet=`mount | grep "on ${JAILDIR} " | awk '{print $1}'`
   zfs unjail $JID $jDataSet
fi

# Check if we need umount x mnts
if [ -e "${JMETADIR}/jail-portjail" ] ; then umountjailxfs ${JAILNAME} ; fi

if [ "$VIMAGEENABLE" = "YES" ] ; then
  jail_interfaces_down "${JID}"
else
  # Get list of IP4s for this jail
  if [ -e "${JMETADIR}/ipv4" ] ; then
    IP4S="`cat ${JMETADIR}/ipv4 | cut -d '/' -f 1`"
  fi
  if [ -e "${JMETADIR}/alias-ipv4" ] ; then
    while read line
    do
      IP4S="${IP4S} `echo $line | cut -d '/' -f 1`"
    done < ${JMETADIR}/alias-ipv4
  fi

  # Get list of IP6s for this jail
  if [ -e "${JMETADIR}/ipv6" ] ; then
    IP6S="`cat ${JMETADIR}/ipv6 | cut -d '/' -f 1`"
  fi
  if [ -e "${JMETADIR}/alias-ipv6" ] ; then
    while read line
    do
      IP6S="${IP6S} `echo $line | cut -d '/' -f 1`"
    done < ${JMETADIR}/alias-ipv6
  fi
 
 
  # Check if we need to remove the IP aliases from this jail
  for _ip in $IP4S
  do 
    # See if active alias
    ifconfig $IFACE | grep -q "${_ip}"
    if [ $? -ne 0 ] ; then continue ; fi

    ifconfig $IFACE inet -alias ${_ip}
  done

  for _ip in $IP6S
  do 
    # See if active alias
    ifconfig $IFACE | grep -q "${_ip}"
    if [ $? -ne 0 ] ; then continue ; fi

    ifconfig $IFACE inet6 ${_ip} delete
  done
fi

if [ -e "${JMETADIR}/jail-linux" ] ; then LINUXJAIL="YES" ; fi

# Check for user-supplied mounts
if [ -e "${JMETADIR}/fstab" ] ; then
   echo "Unmounting user-supplied file-systems"
   cp ${JMETADIR}/fstab /tmp/.wardenfstab.$$
   sed -i '' "s|%%JAILDIR%%|${JAILDIR}|g" /tmp/.wardenfstab.$$
   umount -a -F /tmp/.wardenfstab.$$
   rm /tmp/.wardenfstab.$$
fi

if [ "$LINUXJAIL" = "YES" ] ; then
  # If we have a custom stop script
  if [ -e "${JMETADIR}/jail-stop" ] ; then
    sCmd=`cat ${JMETADIR}/jail-stop`
    echo "Stopping jail with: ${sCmd}"
    if [ -n "${JID}" ] ; then
      jexec ${JID} ${sCmd} 2>&1
    fi
  else
    # Check for different init styles
    if [ -e "${JAILDIR}/etc/init.d/rc" ] ; then
      if [ -n "${JID}" ] ; then
        jexec ${JID} /bin/sh /etc/init.d/rc 0 2>&1
      fi
    elif [ -e "${JAILDIR}/etc/rc" ] ; then
      if [ -n "${JID}" ] ; then
        jexec ${JID} /bin/sh /etc/rc 0 2>&1
      fi
    fi
  fi
  sleep 3

  umount -f ${JAILDIR}/sys 2>/dev/null
  umount -f ${JAILDIR}/dev/fd 2>/dev/null
  umount -f ${JAILDIR}/dev 2>/dev/null
  umount -f ${JAILDIR}/lib/init/rw 2>/dev/null
else
  # If we have a custom stop script
  if [ -e "${JMETADIR}/jail-stop" ] ; then
    if [ -n "${JID}" ] ; then
      sCmd=`cat ${JMETADIR}/jail-stop`
      echo "Stopping jail with: ${sCmd}"
      jexec ${JID} ${sCmd} 2>&1
    fi
  else
    if [ -n "${JID}" ] ; then
      echo "Stopping jail with: /etc/rc.shutdown"
      jexec ${JID} /bin/sh /etc/rc.shutdown >/dev/null 2>/dev/null
    fi
  fi
fi

umount -f ${JAILDIR}/dev >/dev/null 2>/dev/null

echo -e ".\c"

# Skip the time consuming portion if we are shutting down
if [ "$FAST" != "Y" ]
then

# We asked nicely, so now kill the jail for sure
killall -j ${JID} -TERM 2>/dev/null
sleep 1
killall -j ${JID} -KILL 2>/dev/null

echo -e ".\c"

# Check if we need to unmount the devfs in jail
mount | grep "${JAILDIR}/dev" >/dev/null 2>/dev/null
if [ "$?" = "0" ]
then
  # Setup a 60 second timer to try and umount devfs, since takes a bit
  SEC="0"
  while
   i=1
  do
   sleep 2

   # Try to unmount dev
   umount -f "${JAILDIR}/dev" 2>/dev/null
   if [ "$?" = "0" ]
   then
      break
   fi

   SEC="`expr $SEC + 2`"
   echo -e ".\c"

   if [ ${SEC} -gt 60 ]
   then
      break
   fi

  done
fi

# Check if we need to unmount any extra dirs
mount | grep "${JAILDIR}/proc" >/dev/null 2>/dev/null
if [ "$?" = "0" ]; then
  umount -f "${JAILDIR}/proc"
fi

if [ -e "${JMETADIR}/jail-portjail" ] ; then
  umountjailxfs
fi

fi # End of FAST check

echo -e ".\c"

if [ -n "${JID}" ] ; then
  jail -r ${JID}
fi

echo -e "Done"

# Send notification of jail changes
pc-systemflag WARDENUPDATE SUCCESS
