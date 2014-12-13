#!/bin/sh
#######################################################################

# Source our functions
PROGDIR="/usr/local/share/warden"

# Source our variables
. ${PROGDIR}/scripts/backend/functions.sh

JAILNAME="${1}"

if [ -z "${JAILNAME}" ]
then
  echo "ERROR: No jail specified to start!"
  exit 5
fi

if [ -z "${JDIR}" ]
then
  echo "ERROR: JDIR is unset!!!!"
  exit 5
fi

JAILDIR="${JDIR}/${JAILNAME}"
NEWJAILDIR="${JDIR}/${NEWJAILNAME}"

if [ ! -d "${JAILDIR}" ]
then
  echo "ERROR: No jail located at ${JAILDIR}"
  exit 5
fi

# Now start the pkg upgrade process
update_pkgs_only
res=$?

exit $res
