#!/bin/sh
# Script to stop a jail
# Args $1 = jail-dir
#######################################################################

# Source our functions
PROGDIR="/usr/local/share/warden"

# Source our variables
. ${PROGDIR}/scripts/backend/functions.sh

JAILNAME="$1"

if [ -z "${JAILNAME}" ]
then
  echo "ERROR: No jail specified to delete!"
  rtn
  exit 5
fi

if [ -z "${JDIR}" ]
then
  echo "ERROR: JDIR is unset!!!!"
  rtn
  exit 5
fi

JAILDIR="${JDIR}/${JAILNAME}"

if [ ! -d "${JAILDIR}" ] ; then
   echo "ERROR: No such jail at ${JAILDIR}"
   exit 5
fi

set_warden_metadir

# Check if the jail is running first
${PROGDIR}/scripts/backend/checkstatus.sh "${JAILNAME}"
if [ "$?" = "0" ]
then
  # Make sure the jail is stopped
  ${PROGDIR}/scripts/backend/stopjail.sh "${JAILNAME}"
fi

# Confirm jail was shutdown and no mounts are left
${PROGDIR}/scripts/backend/checkstatus.sh "${JAILNAME}"
if [ "$?" = "0" ] ; then
   echo "ERROR: Jail is still running! Please stop manually."
   exit 5
fi

# Check if anything is still mounted in this jail
hasmount=0
for mountpoint in $(mount | grep -e "${JAILDIR}/" | cut -d" " -f3); do
  hasmount=1
done
if [ $hasmount -eq 1 ] ; then
   echo "ERROR: Jail still has active mount points. Please unmount them manually."
   exit 5
fi


echo -e "Deleting Jail...\c"
isDirZFS "${JAILDIR}" "1"
if [ $? -eq 0 ] ; then
  # Get the dataset of the jails mountpoint
  jDataSet=`mount | grep "on ${JAILDIR} " | awk '{print $1}'`

  # Delete ZFS mount
  umount -f "${JAILDIR}" >/dev/null 2>/dev/null
  zfs destroy -r ${jDataSet}
  if [ $? -ne 0 ] ; then
     # Remount the dataset so we can try again later
     mount -t zfs $jDataSet $JAILDIR
     exit_err "Failed destroying $jDataSet"
     exit 1
  fi
  rmdir ${JAILDIR} 2>/dev/null
else
  chflags -R noschg "${JAILDIR}"
  rm -rf "${JAILDIR}"
fi

if [ ! -z "${JMETADIR}" -a "${JMETADIR}" != " " ] ; then
  rm -rf "${JMETADIR}"
fi

echo "Done"

# Send notification of jail changes
pc-systemflag WARDENUPDATE SUCCESS
