#!/bin/sh
# Script for Life-Preserver which starts a backup on the specified host
###########################################################################
SSHHOST=$1

if [ -z "${SSHHOST}" ]
then
  echo "ERROR: Usage start-backup.sh <user>@<host>"
  exit 150
fi

DATE=`date "+%Y-%m-%dT%H_%M_%S"`
PROGDIR="/usr/local/share/lifePreserver"
HOSTDIR="${PROGDIR}/preservers/${SSHHOST}"
BACKLOG="${HOSTDIR}/logs/${DATE}.log"
EXCLUDELIST="/usr/local/share/lifePreserver/conf/rsync-excludes"
USEREXCLUDELIST="${HOSTDIR}/exclude-list"
TMPEXCLUDELIST="${HOSTDIR}/.working-exclude-list"
INCLUDELIST="/usr/local/share/lifePreserver/conf/rsync-includes"
USERINCLUDELIST="${HOSTDIR}/include-list"
TMPINCLUDELIST="${HOSTDIR}/.working-include-list"
PIDFILE="${HOSTDIR}/preserver.pid"
RESULTFILE="${HOSTDIR}/last-result"
STARTDATE=`date "+%H:%M %m-%d-%y"`
SETTINGS="${HOSTDIR}/settings.conf"

# Source our functions
. ${PROGDIR}/conf/life-preserver.conf
. ${PROGDIR}/scripts/functions.sh

if [ -e "${HOSTDIR}/remote-dir" ] ; then
  BACKDIR="`cat ${HOSTDIR}/remote-dir`"
else
  BACKDIR="${DEFAULT_BACKDIR}"
fi

# Make sure we've been given a valid life preserver
if [ ! -d "${HOSTDIR}" ] ; then
  echo "ERROR: Invalid preserver: ${SSHHOST}"
  exit 1
fi

# Copy over the exclude files to our working list
cp ${EXCLUDELIST} ${TMPEXCLUDELIST}
if [ -e "${USEREXCLUDELIST}" ] ; then
  cat ${EXCLUDELIST} ${USEREXCLUDELIST} > ${TMPEXCLUDELIST}
fi

# Copy over the include files to our working list
cp ${INCLUDELIST} ${TMPINCLUDELIST}
if [ -e "${USERINCLUDELIST}" ] ; then
  cat ${INCLUDELIST} ${USERINCLUDELIST} > ${TMPINCLUDELIST}
fi

# Setup our backup list
while read bline
do
	BACKUPLIST="$BACKUPLIST $bline"
done < $TMPINCLUDELIST


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
check_pid "`cat ${PIDFILE}`"
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

rsync -avvRHz \
  --delete \
  --delete-excluded \
  --rsync-path="rsync --fake-super" \
  --perms --chmod=o-t \
  --log-file=${BACKLOG} \
  --exclude-from ${TMPEXCLUDELIST} \
  -e "ssh -p ${PORT}" \
  --link-dest=../current \
  ${BACKUPLIST} ${SSHHOST}:${BACKDIR}/incomplete_back-$DATE

RESULT="$?"

# Check the result, if 0, 23 or 24, we can take it as successful backup
# This gets rid of failing because of a file disappearing during the backup
if [ "$RESULT" = "0" -o "$RESULT" = "24" -o "$RESULT" = "23" ]
then
  # Success!
  echo "Success: ${STARTDATE}" >${RESULTFILE}
  date +%y:%m:%d >${HOSTDIR}/last-timestamp

  ssh -p ${PORT} ${SSHHOST} \
  "mv ${BACKDIR}/incomplete_back-$DATE ${BACKDIR}/back-$DATE \
  ; touch ${BACKDIR}/back-$DATE \
  ; rm -f ${BACKDIR}/current \
  ; ln -s back-${DATE} ${BACKDIR}/current"

else
  # Failed backup!
  echo "Failed: ${STARTDATE}" >${RESULTFILE}
fi

# Check if we need to cleanup any failed backups
if [ "$CLEANFAILED" = "true" ]
then
  echo "Cleaning up incomplete backups..."
  echo "Cleaning up incomplete backups..." >>${BACKLOG}
  ssh -p ${PORT} ${SSHHOST} \
  "chmod -R 777 ${BACKDIR}/incomplete_back-* \
; rm -rf ${BACKDIR}/incomplete_back-*" >>${BACKLOG} 2>>${BACKLOG} 
fi

# Check if we need to prune any old saved backups
if [ ! -z "${KEEPNUM}" ]
then
  COUNT="0"

  # Get a listing of the number of full backups saved
  OLDBACKUPS=`ssh -p ${PORT} ${SSHHOST} "ls -dt ${BACKDIR}/back-*"`
  if [ "$?" = "0" ]
  then
    for i in ${OLDBACKUPS}
    do
      COUNT="`expr $COUNT + 1`"
      if [ $COUNT -gt $KEEPNUM -a ! -z "${i}" ]
      then
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
