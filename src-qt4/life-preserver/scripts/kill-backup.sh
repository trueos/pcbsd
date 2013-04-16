#!/bin/sh
# Script which kills a specific backup in progress

PRESERVER=$1

if [ -z "${PRESERVER}" ]
then
  echo "ERROR: Usage kill-backup.sh <user>@<host>"
  exit 150
fi

PROGDIR="/usr/local/share/lifePreserver"
PDIR="${PROGDIR}/preservers/${PRESERVER}"
SETTINGS="${PDIR}/settings.conf"

# Source our functions
. ${PROGDIR}/scripts/functions.sh

# Make sure we've been given a valid life preserver
if [ ! -d "${PDIR}" ]
then
  echo "ERROR: Invalid preserver: ${PHOST}"
  exit 1
fi

cat ${SETTINGS} | grep "type: rsync" >/dev/null 2>/dev/null
if [ "$?" = "0" ]
then
  PID=`ps -auwwwx | grep "rsync" | grep "fake-super" | grep "${PRESERVER}" | tr -s ' ' | cut -d ' ' -f 2`
  if [ ! -z "${PID}" ]
  then
    kill -9 ${PID}
  fi
fi

