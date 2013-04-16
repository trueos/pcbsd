#!/bin/sh
# Script for Life-Preserver which lists the backups present on a server
###########################################################################
SSHHOST=$1

if [ -z "${SSHHOST}" ]
then
  echo "ERROR: Usage list-backup-rsync.sh <user>@<host>"
  exit 150
fi

PROGDIR="/usr/local/share/lifePreserver"
HOSTDIR="${PROGDIR}/preservers/${SSHHOST}"
PIDFILE="${HOSTDIR}/preserver.pid"
SETTINGS="${HOSTDIR}/settings.conf"

# Source our functions
. ${PROGDIR}/conf/life-preserver.conf
. ${PROGDIR}/scripts/functions.sh

# Make sure we've been given a valid life preserver
if [ ! -d "${HOSTDIR}" ]
then
  echo "ERROR: Invalid preserver: ${SSHHOST}"
  exit 1
fi

if [ -e "${HOSTDIR}/remote-dir" ] ; then
  BACKDIR="`cat ${HOSTDIR}/remote-dir`"
else
  BACKDIR="${DEFAULT_BACKDIR}"
fi

# Start by reading in some settings from the config
get_setting ${SETTINGS} "port"
PORT="${VAL}"

# Make sure we have a sane port number
if [ -z "${PORT}" ]
then
  PORT="22"
fi

# Make sure we can connect
ssh -p ${PORT} ${SSHHOST} -o BatchMode=yes "ls" >/dev/null 2>/dev/null
if [ $? -ne 0 ]; then
  echo "Failed to connect! ${STARTDATE}"
  exit 1
fi

# Get a listing of the number of full backups saved
OLDBACKUPS=`ssh -p ${PORT} ${SSHHOST} "ls -dt ${BACKDIR}/back-*"`
if [ "$?" = "0" ]
then
  for i in ${OLDBACKUPS}
  do
    DIR=`echo ${i} | sed "s|^${BACKDIR}/||g"`
    if [ -z "${BACKLIST}" ]
    then
      BACKLIST="${DIR}"
    else
      BACKLIST="${BACKLIST}:${DIR}"
    fi
  done

  if [ -z "${BACKLIST}" ]
  then
    echo "NONE"
  else
    echo "$BACKLIST"
  fi

else
  echo "FAILED"  
fi
