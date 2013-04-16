#!/bin/sh
# Script for Life-Preserver which starts a backup on the specified host
###########################################################################
PRESERVER=$1

PATH="${PATH}:/usr/local/bin:/usr/local/sbin"
export PATH

if [ -z "${PRESERVER}" ]
then
  echo "ERROR: Usage start-backup.sh <user>@<host>"
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

get_setting ${SETTINGS} "type"
TYPE="${VAL}"
if [ "$TYPE" = "rsync" ]
then
  ${PROGDIR}/scripts/backup-rsync.sh ${PRESERVER}
else
  echo "ERROR: Invalid type: specified"
  exit 1
fi

