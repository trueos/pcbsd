#!/bin/sh
# Export a jail into a self-contained file for transport / backup
######################################################################

# Source our functions
PROGDIR="/usr/local/share/warden"

# Source our variables
. ${PROGDIR}/scripts/backend/functions.sh

EXPORTNAME="$1"
JAILNAME="$1"
OUTDIR="$2"

if [ -z "${OUTDIR}" ]; then OUTDIR="$WTMP" ; fi

if [ -z "${EXPORTNAME}" ]
then
  echo "ERROR: No jail specified to export!"
  exit 5
fi

if [ -z "${JDIR}" ]
then
  echo "ERROR: JDIR is unset!!!!"
  exit 5
fi

JAILDIR="${JDIR}/${EXPORTNAME}"

if [ ! -d "${JAILDIR}" ]
then
  echo "ERROR: No jail located at ${JAILDIR}"
  exit 5
fi

set_warden_metadir

# First check if this jail is running, and stop it
${PROGDIR}/scripts/backend/checkstatus.sh "${EXPORTNAME}"
if [ "$?" = "0" ]
then
  echo "Stopping jail for export..."
  ${PROGDIR}/scripts/backend/stopjail.sh "${EXPORTNAME}"
fi

# Reset JAILDIR
JAILNAME="$1"
JAILDIR="${JDIR}/${EXPORTNAME}"
set_warden_metadir

# Now that the jail is stopped, lets make a large tbz file of it
cd ${JAILDIR}

# Get the Hostname
HOST="`cat ${JMETADIR}/host`"

IP4="`cat ${JMETADIR}/ipv4 2>/dev/null`"
IP6="`cat ${JMETADIR}/ipv6 2>/dev/null`"

get_ip_and_netmask "${IP4}"
IP4="${JIP}"
MASK4="${JMASK}"

get_ip_and_netmask "${IP6}"
IP6="${JIP}"
MASK6="${JMASK}"

if [ ! -d "$OUTDIR" ] ; then
  mkdir -p ${OUTDIR}
fi

TDIR="`mktemp -d ${WTMP}/exportJail-XXXXXXXXX`"
cd ${TDIR}

# Copy over jail extra meta-data
cp ${JMETADIR}/* ${TDIR}/ 2>/dev/null

# Compress the info dir
cd ${TDIR}
tar cvJf "${JAILDIR}/jailinfo.txz" . 2>/dev/null
if [ $? -ne 0 ] ; then
   exit_err "Failed creating jail info file"
fi

# Cleanup the old TDIR
cd ${OUTDIR}
rm -rf "$TDIR"

echo "Creating compressed archive of ${EXPORTNAME}... Please Wait..."
tar cvJf "${OUTDIR}/${EXPORTNAME}.wdn" -C "${JAILDIR}" . 2>/dev/null
if [ $? -ne 0 ] ; then
   exit_err "Failed creating warden archive"
fi

rm "${JAILDIR}/jailinfo.txz" 2>/dev/null

echo "Created ${EXPORTNAME}.wdn in ${OUTDIR}" >&1

exit 0
