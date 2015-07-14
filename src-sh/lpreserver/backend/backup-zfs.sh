#!/bin/sh
# Script for Life-Preserver which starts a backup on the specified host
# Modified 7/14/2015 by Joshua Smith
###########################################################################
SSHHOST=$1

if [ -z "${SSHHOST}" ]
then
  echo "ERROR: Usage start-backup.sh <user>@<host>"
  exit 150
fi

DATE=`date "+%Y-%m-%dT%H_%M_%S"`
PROGDIR="/usr/local/share/lifePreserver"
HOSTDIR="/var/run/preservers/${SSHHOST}"
BACKLOG="${HOSTDIR}/logs/${DATE}.log"
PIDFILE="${HOSTDIR}/preserver.pid"
RESULTFILE="${HOSTDIR}/last-result"
STARTDATE=`date "+%H:%M %m-%d-%y"`
SETTINGS="${HOSTDIR}/settings.conf"

# Source our functions
. ${PROGDIR}/conf/life-preserver.conf
. ${PROGDIR}/scripts/functions.sh
. /usr/local/share/pcbsd/scripts/functions.sh

prepZFS() {
  isDirZFS "${1}" "1"
  if [ $? -ne 0 ] ; then
     exit_err "Not a ZFS volume: ${1}"
  fi
  zdate=`date +%Y-%m-%d-%H-%M-%S`
  tank=`getZFSTank "$1"`
  rp=`getZFSRelativePath "$1"`
  export zdate tank rp
}

sendZFSSnap() {
  echo "Sending snapshot ${tank}${rp}@${zdate}"
  zfs send -RPv ${tank}${rp}@${zdate} >/dev/null
  #zfs send -RPv ${tank}${rp}@${zdate} | gzip | ssh -p ${PORT} ${SSHHOST} "cat > ${BACKDIR}/incomplete_snap-${DATE}.gz" 
  return $?
}

mkZFSSnap() {
  zfs snapshot -r $tank${rp}@$zdate
  return $?
}

if [ -e "${HOSTDIR}/remote-dir" ] ; then
  BACKDIR="`cat ${HOSTDIR}/remote-dir`"
else
  BACKDIR="${DEFAULT_BACKDIR}"
fi

# KPM - REMOVE ME
BACKDIR="zfssnaps"

# Make sure we've been given a valid life preserver
if [ ! -d "${HOSTDIR}" ] ; then
  echo "ERROR: Invalid preserver: ${SSHHOST}"
  exit 1
fi

# Start by reading in some settings from the config
get_setting ${SETTINGS} "keepbackups"
KEEPNUM="${VAL}"
get_setting ${SETTINGS} "port"
PORT="${VAL}"
get_setting ${SETTINGS} "cleanfailed"
CLEANFAILED="${VAL}"

# Make sure we have a sane port number
if [ -z "${PORT}" ] ; then
  PORT="22"
fi

# Make sure this host has its logs dir
if [ ! -d "${HOSTDIR}/logs" ] ; then
  mkdir ${HOSTDIR}/logs
fi

# Save the PID of this running process
check_pid "`cat ${PIDFILE} 2>/dev/null`"
if [ "$?" = "1" ] ; then
  exit 150
else
  echo "$$" >${PIDFILE}
fi

# Make sure we can connect
ssh -p ${PORT} ${SSHHOST} -o BatchMode=yes "ls" >/dev/null 2>/dev/null
if [ $? -ne 0 ]; then
  echo "Failed to connect! ${STARTDATE}"
  echo "Failed to connect! ${STARTDATE}" >${RESULTFILE}
  exit 1
fi

# Make sure we have the BACKDIR created
ssh -p ${PORT} ${SSHHOST} "mkdir -p ${BACKDIR}"

# Lets create a snapshot & send it
prepZFS "/usr/home"
if ! mkZFSSnap ; then
  exit_err "Failed to create ZFS snapshot"
fi
sendZFSSnap
if [ "$?" = "0" ]; then
  # Success!
  echo "Success: ${STARTDATE}" >${RESULTFILE}
  date +%y:%m:%d >${HOSTDIR}/last-timestamp

  #ssh -p ${PORT} ${SSHHOST} \
  #"mv ${BACKDIR}/incomplete_snap-$DATE ${BACKDIR}/snap-$DATE.gz \
  #; rm -f ${BACKDIR}/current.gz \
  #; ln -s snap-${DATE}.gz ${BACKDIR}/current.gz"

else
  # Failed backup!
  echo "Failed: ${STARTDATE}" >${RESULTFILE}
fi

echo "Removing: ${tank}${rp}@${zdate}"
zfs destroy -r ${tank}${rp}@${zdate}
exit 0

# Check if we need to cleanup any failed backups
if [ "$CLEANFAILED" = "true" ]; then
  echo "Cleaning up incomplete backups..."
  echo "Cleaning up incomplete backups..." >>${BACKLOG}
  ssh -p ${PORT} ${SSHHOST} \
  "chmod -R 777 ${BACKDIR}/incomplete_back-* \
; rm -rf ${BACKDIR}/incomplete_back-*" >>${BACKLOG} 2>>${BACKLOG} 
fi

# Check if we need to prune any old saved backups
if [ ! -z "${KEEPNUM}" ]; then
  COUNT="0"

  # Get a listing of the number of full backups saved
  OLDBACKUPS=`ssh -p ${PORT} ${SSHHOST} "ls -dt ${BACKDIR}/back-*"`
  if [ "$?" = "0" ]; then
    for i in ${OLDBACKUPS}
    do
      COUNT="`expr $COUNT + 1`"
      if [ $COUNT -gt $KEEPNUM -a ! -z "${i}" ]; then
        # We've found a backup to delete, do it now
        echo "Removing old backup: ${i}"
        echo "Removing old backup: ${i}" >>${BACKLOG}
        ssh -p ${PORT} ${SSHHOST} "chflags -R noschg ${i} \
        ; chmod -R 777 ${i} \
        ; rm -rf ${i}" >>${BACKLOG} 2>>${BACKLOG}
      fi
    done 
  fi
fi

# Compress our logfile to save disk space
bzip2 ${BACKLOG}

# Remove the PID
rm ${PIDFILE}
