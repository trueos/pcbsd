#!/bin/sh
# ZFS functionality
# Args $1 = DATASET
# Args $2 = zfs directive
#######################################################################

# Source our functions
PROGDIR="/usr/local/share/lpreserver"

# Source our variables
. /usr/local/share/pcbsd/scripts/functions.sh
. ${PROGDIR}/backend/functions.sh

DATASET="${1}"
 
if [ -z "${DATASET}" ]; then
  exit_err "No dataset specified!"
fi

revertZFSSnap "${DATASET}" "${SNAP}"
