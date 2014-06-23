#!/bin/sh
# ZFS functionality
# Args $1 = jail-name
# Args $2 = new jail name
# Args $3 = new IPV4
# Args $4 = new IPV6
#######################################################################

# Source our functions
PROGDIR="/usr/local/share/warden"

# Source our variables
. ${PROGDIR}/scripts/backend/functions.sh

JAILNAME="${1}"
NEWJAILNAME="${2}"
IPV4="${3}"
IPV6="${4}"

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

if [ -d "${NEWJAILDIR}" ] ; then
   echo "ERROR: $NEWJAILNAME already exists!"
   exit 1
fi


# Create a new snap and clone the original jail
cloneZFSDir "$JAILDIR" "$NEWJAILDIR"
if [ $? -ne 0 ] ; then
  exit_err "Failed doing jail clone!"
fi

# Get the warden meta-dirs
set_warden_metadir
NEWJMETADIR="${JDIR}/.${NEWJAILNAME}.meta"

# Copy the meta-dir
cp -r "$JMETADIR" "${NEWJMETADIR}"
if [ $? -ne 0 ] ; then
   exit_err "Failed creating new jail meta-dir"
fi

# Set some of the new data
echo "$NEWJAILNAME" > ${NEWJMETADIR}/host

# Set IPV4
if [ "$IPV4" = "OFF" ] ; then
   rm ${NEWJMETADIR}/ipv4 2>/dev/null
else
   echo "$IPV4" > ${NEWJMETADIR}/ipv4
fi

# Set IPV6
if [ "$IPV6" = "OFF" ] ; then
   rm ${NEWJMETADIR}/ipv6 2>/dev/null
else
   echo "$IPV6" > ${NEWJMETADIR}/ipv6
fi

exit 0
