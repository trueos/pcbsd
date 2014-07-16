#!/bin/sh
# Install a package set into a jail
######################################################################

# Source our functions
PROGDIR="/usr/local/share/warden"

# Source our variables
. ${PROGDIR}/scripts/backend/functions.sh

JAILNAME="$1"
IFILE="$2"
HOST="$3"
IP4="$4"
IP6="$5"

# Get path to IFILE
chk=`echo "${IFILE}" | cut -c 1-1`
if [ "$chk" != "/" ]; then IFILE="`pwd`/${IFILE}"; fi

# Do all the sanity checks
if [ -z "${IFILE}" -o ! -e "${IFILE}" ]; then exit_err "ERROR: No such file ${IFILE}!"; fi
if [ -z "${JDIR}" ]; then exit_err "ERROR: JDIR is unset!!!!"; fi
JAILDIR="${JDIR}/${JAILNAME}"
if [ -e "${JAILDIR}" ] ; then exit_err "Jail: $JAILNAME already exists!"; fi

# Lets start importing the jail now
######################################################################

# Figure out what style / version of export data this is
IMPORTVER=2
tar tvfq ${IFILE} ./jailinfo.txz >/dev/null 2>/dev/null
if [ $? -ne 0 ]; then
    # CHeck if this is an old-style export file
    grep -q '^___WARDEN_START___' ${IFILE}
    if [ $? -eq 0 ] ; then
       SKIP="`awk '/^___WARDEN_START___/ { print NR + 1; exit 0; }' ${IFILE}`"
       IMPORTVER=1
    else
      tar tvf ${IFILE} >/dev/null 2>/dev/null
      if [ $? -ne 0 ]; then
         exit_err "Cannot import jail, invalid / corrupt tar archive!"
      fi
       IMPORTVER=0
    fi
fi

cd ${JDIR}
echo "Importing ${IFILE} into jail ${JAILNAME}"

# Get the dataset of the jails mountpoint
rDataSet=`mount | grep "on ${JDIR} " | awk '{print $1}'`
tSubDir=`basename $JAILDIR`
nDataSet="${rDataSet}/${tSubDir}"

zfs create -p ${nDataSet}
if [ $? -ne 0 ] ; then
   exit_err "Failed creating ZFS dataset for jail ${JAILDIR}"
fi

# Get next unique ID
META_ID=0
for i in `ls -d ${JDIR}/.*.meta 2>/dev/null`
do
  id=`cat ${i}/id`
  if [ "${id}" -gt "${META_ID}" ] ; then
    META_ID="${id}"
  fi
done
: $(( META_ID += 1 ))

# Create the meta-dir
set_warden_metadir
mkdir ${JMETADIR}

# Extract the jail contents
case $IMPORTVER in
    2) tar xvfq ${IFILE} -C ${JMETADIR} ./jailinfo.txz >/dev/null 2>/dev/null
       if [ $? -ne 0 ] ; then echo "WARNING: Failed to locate meta-data..."; fi
       tar xvf ${JMETADIR}/jailinfo.txz -C ${JMETADIR} >/dev/null 2>/dev/null
       if [ $? -ne 0 ] ; then echo "WARNING: Failed to extract meta-data..."; fi
       rm ${JMETADIR}/jailinfo.txz >/dev/null 2>/dev/null
       tar xpJf ${IFILE} -C "${JAILDIR}" --exclude ./jailinfo.txz 2>/dev/null
       if [ $? -ne 0 ] ; then echo "WARNING: Failed to fully extract jail..."; fi
       ;;
    1) tail +${SKIP} ${IFILE} | tar xpf - -C "${JAILDIR}" 2>/dev/null
       if [ $? -ne 0 ] ; then echo "WARNING: Failed to fully extract jail..."; fi
       ;;
    0) tar xpf ${IFILE} -C "${JAILDIR}" 2>/dev/null
       if [ $? -ne 0 ] ; then echo "WARNING: Failed to fully extract jail..."; fi
       ;;
    *) exit_err "Unknown import version..." ;;
esac

# give new jail an id
echo "${META_ID}" > ${JMETADIR}/id

# Make sure we have an IP address saved
if [ "${IP4}" != "OFF" ] ; then
  echo "${IP4}" >"${JMETADIR}/ipv4"
fi
if [ "${IP6}" != "OFF" ] ; then
  echo "${IP6}" >"${JMETADIR}/ipv6"
fi

# Create the hostname for this device
if [ "${HOST}" != "OFF" ]; then
  echo "${HOST}" >"${JMETADIR}/host"
else
  echo "${JAILNAME}" >"${JMETADIR}/host"
fi

echo "Done"
