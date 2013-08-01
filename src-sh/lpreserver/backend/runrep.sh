#!/bin/sh
# Do the replication for a specific dataset
######################################################################

# Set our vars
PROGDIR="/usr/local/share/lpreserver"

# Source our functions
. /usr/local/share/pcbsd/scripts/functions.sh
. ${PROGDIR}/backend/functions.sh

DATASET="${1}"

if [ -z "${DATASET}" ]; then
  exit_err "No dataset specified!"
fi

# Lets start by building a list of props to keep
pTag=`echo $DATASET | md5`

if [ "$RECURMODE" = "ON" ] ; then
   zfs get -r all $DATASET | grep ' local$' | awk '{$1=$1}1' OFS=" " > /tmp/.propList.$$
else
   zfs get all $DATASET | grep ' local$' | awk '{$1=$1}1' OFS=" " > /tmp/.propList.$$
fi

cat /tmp/.propList.$$
rm /tmp/.propList.$$

if [ "$EMAILMODE" = "ALL" ] ; then
   #email_msg "Automated Snapshot" "`echo_queue_msg`"
fi
