#!/bin/sh
# Do the replication for a specific dataset
# Modified 7/14/2015 by Joshua Smith
######################################################################

# Set our vars
PROGDIR="/usr/local/share/lpreserver"

# Source our functions
. /usr/local/share/pcbsd/scripts/functions.sh
. ${PROGDIR}/backend/functions.sh

DATASET="${1}"
rHost="${2}"

if [ -z "${DATASET}" ]; then
  exit_err "No dataset specified!"
fi
if [ -z "${rHost}" ]; then
  exit_err "No target host specified!"
fi

check_rep_task "$DATASET" "$rHost"
status=$?

# No replication was needed / done
if [ $DIDREP -eq 0 ] ; then
   exit 0
fi

if [ "$TIME" = "force" ] ; then
  type="Manual" 
else
  type="Automated" 
fi

if [ $status -eq 0 ] ; then
  title="Success"
else
  title="FAILED"
fi

case $EMAILMODE in
    ALL) email_msg "$title - $type Replication" "`echo_queue_msg`" ;;
    *) if [ $status -ne 0 ] ; then
          email_msg "$title - $type Replication" "`echo_queue_msg`"
       fi
       ;;
esac

rm ${MSGQUEUE} 2>/dev/null
