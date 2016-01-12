#!/bin/sh
# ZFS functionality
# Args $1 = dataset 
# Args $2 = snapshot
#######################################################################

# Source our functions
PROGDIR="/usr/local/share/lpreserver"

# Source our variables
. /usr/local/share/pcbsd/scripts/functions.sh
. ${PROGDIR}/backend/functions.sh

DATASET="${1}"
SNAP="${2}"

if [ -z "${DATASET}" ]; then
  exit_err "No dataset specified!"
fi

if [ -z "${SNAP}" ]; then
  exit_err "No snapshot specified!"
fi

rmZFSSnap "${DATASET}" "${SNAP}"
if [ $? -ne 0 ] ; then
  echo "ERROR: Failed removing snapshot: ${DATASET}@${SNAP}"
  exit 1
fi
exit 0
