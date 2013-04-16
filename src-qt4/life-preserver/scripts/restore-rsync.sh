#!/bin/sh
# Script for Life-Preserver which restores the specified files
###########################################################################
SSHHOST=$1

if [ -z "${SSHHOST}" ]
then
  echo "ERROR: Usage restore-rsync.sh <user>@<host>"
  exit 150
fi

# Load our other args
TARGETBACKUP="${2}"
RESTORELIST="${3}"
RELRESTOREDIR="${4}"
RELKEEPPATH="${5}"


PROGDIR="/usr/local/share/lifePreserver"
HOSTDIR="${PROGDIR}/preservers/${SSHHOST}"
SETTINGS="${HOSTDIR}/settings.conf"
RESTORESH="${HOSTDIR}/.restoreCMD.$$"

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

# Create our restore list which will we suck back in 
TMPRESTOREFILE="/tmp/.restoreList.$$"
echo "${RESTORELIST}" | tr ',' '\n' > $TMPRESTOREFILE

# Check if we've enabled the option to keep the full file path, and setup our restore string
if [ "${RELKEEPPATH}" = "ON" ]
then
  ARGS="-R"
  SPACER="/."
else
  ARGS=""
  SPACER=""
fi

while read line
do
  if [ -z "${RESTORESTR}" ]
  then
    RESTORESTR=":\"${BACKDIR}/${TARGETBACKUP}${SPACER}${line}\""
  else
    RESTORESTR="${RESTORESTR} :\"${BACKDIR}/${TARGETBACKUP}${SPACER}${line}\""
  fi
done < $TMPRESTOREFILE
rm $TMPRESTOREFILE

CMD="rsync -avvzs ${ARGS} \
  --rsync-path=\"rsync --fake-super\" \
  -e \"ssh -p ${PORT}\" \
  ${SSHHOST}${RESTORESTR} ${RELRESTOREDIR}"

echo "${CMD}" >${RESTORESH}
sh ${RESTORESH}
RESULT="$?"
rm ${RESTORESH}

if [ "$RESULT" = "0" ]
then
  # Success!
  echo "Success!"
  exit 0
else
  # Failed restore
  echo "Failed restoring all files..."
  exit 150
fi

