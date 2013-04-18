#!/bin/sh
# Starts up the warden service
######################################################################

PATH="${PATH}:/usr/local/bin:/usr/local/sbin"
export PATH

# Source our functions
PROGDIR="/usr/local/share/warden"

if [ -z "${PROGDIR}" ]
then
   echo "PROGDIR unset! Is The Warden installed properly?"
   exit 155
fi

# Source our variables
. ${PROGDIR}/scripts/backend/functions.sh

echo "Stopping the Warden"

# If no jails we can exit
if [ ! -d "${JDIR}" ] ; then exit 0 ; fi

cd ${JDIR}
for i in `ls -d .*.meta 2>/dev/null`
do
  JAILNAME=`echo ${i}|sed 's|.meta$||'|sed 's|^.||'`

  ${PROGDIR}/scripts/backend/checkstatus.sh "${JAILNAME}" 2>/dev/null
  if [ "$?" = "0" ] ; then
    echo "Stopping jail (${JAILNAME})"
    warden stop "${JAILNAME}" "FAST"
  fi
done

