#!/bin/sh
#-
# Copyright (c) 2013 iXsystems, Inc.  All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#

# Functions which runs commands on the system

. ${BACKEND}/functions.sh
. ${BACKEND}/functions-parse.sh

do_zfs_restore()
{
  load_ssh_settings
  
  restore_zfs_from_remote

  restore_stamp_grub

  restore_umount_zfs
}

# Function to load & check SSH remote settings
load_ssh_settings()
{
  get_value_from_cfg sshHost
  SSHHOST="$VAL"
  get_value_from_cfg sshPort
  SSHPORT="$VAL"
  get_value_from_cfg sshUser
  SSHUSER="$VAL"
  get_value_from_cfg sshKey
  SSHKEY="$VAL"

  if [ -z "$SSHHOST" ] ; then
     exit_err "Missing sshHost!"
  fi
  if [ -z "$SSHUSER" ] ; then
     exit_err "Missing sshUser!"
  fi
  if [ -z "$SSHPORT" ] ; then
     SSHPORT="22"
  fi
  if [ -n "$SSHKEY" ] ; then
     SSHKEY="-i $SSHKEY"
  fi

  # Now that the settings are loaded, lets test this connection
  ssh -o NumberOfPasswordPrompts=0 $SSHKEY -p $SSHPORT ${SSHUSER}@${SSHHOST} "echo ''"
  if [ $? -ne 0 ] ; then
     exit_err "Failed SSH connection to ${SSHHOST}"
  fi

  export SSHKEY SSHPORT SSHUSER SSHHOST
}

restore_zfs_from_remote()
{
  get_value_from_cfg zfsProps
  ZFSPROPS="$VAL"
  if [ -z "$ZFSPROPS" ] ; then
     exit_err "No zfsProps= specified!"
  fi

  ZPFILE="${TMPDIR}/.zfs-restore-props"

  # First lets fetch the ZFS properties backup
  echo "ssh -p ${SSHPORT} ${SSHKEY} ${SSHUSER}@${SSHHOST} \"cat '$ZFSPROPS'\" > ${ZPFILE}" > ${TMPDIR}/.fetchProp
  sh ${TMPDIR}/.fetchProp
  if [ $? -ne 0 ] ; then
     rm ${TMPDIR}/.fetchProp
     exit_err "Unable to get ZFS properties file!"
  fi
  rm ${TMPDIR}/.fetchProp

  get_value_from_cfg zfsRemoteDataset
  ZFSDATASET="$VAL"

  lastSNAP=`ssh -p $SSHPORT ${SSHKEY} ${SSHUSER}@${SSHHOST} "zfs list -t snapshot -d 1 -H ${ZFSDATASET}" | tail -1 | awk '{$1=$1}1' OFS=" " | cut -d '@' -f 2 | cut -d ' ' -f 1`

  echo "lastSnap: $lastSNAP"
  sleep 5
   
  # Lets start pulling our ZFS replication
  zSEND="ssh -p $SSHPORT ${SSHKEY} ${SSHUSER}@${SSHHOST} zfs send -Rv ${ZFSDATASET}@${lastSNAP}"
  zRECV="zfs receive -evuF ${ZPOOLNAME}"
  $zSEND | $zRECV >/dev/null 2>/dev/null
  if [ $? -ne 0 ] ; then
     exit_err "Failed ZFS send / receive"
  fi

  # OK, with all the ZFS datasets received, we can now rename
  echo_log "Moving datasets to correct locations"
  for i in `zfs list -d 2 $ZPOOLNAME | grep -v "^NAME" | awk '{print $1}'`
  do
    tDir=`basename ${i}`
    if [ "$tDir" = "$i" ]; then continue ; fi

    rc_nohalt "zfs rename -u ${i} ${ZPOOLNAME}/${tDir}"
  done

  # Lets create our placeholder snapshots on the zpool
  echo_log "Cloning snapshots on zpool.."
  hName=`basename $ZFSDATASET`
  zfs list -d 1 -H -t snapshot ${ZPOOLNAME}/${hName} >${TMPDIR}/.zsnaps
  while read line
  do 
    zSnap="`echo $line | awk '{print $1}' | cut -d '@' -f 2`"
    rc_halt "zfs snapshot ${ZPOOLNAME}@$zSnap"
  done < ${TMPDIR}/.zsnaps
  rm ${TMPDIR}/.zsnaps

  # Delete the old hostname dataset
  rc_halt "zfs destroy -r ${ZPOOLNAME}/${hName}"

  # Lets export / import the pool
  rc_halt "cp /boot/zfs/zpool.cache ${TMPDIR}/zpool.cache"
  rc_halt "zpool export ${ZPOOLNAME}"
  rc_halt "zpool import -N -R ${FSMNT} ${ZPOOLNAME}"

  # Lets mount the default dataset
  lastBE="`zfs list | grep ${ZPOOLNAME}/ROOT/ | tail -n 1 | awk '{print $1}'`"
  rc_halt "mount -t zfs ${lastBE} ${FSMNT}"

  echo_log "Setting ZFS dataset properties.."
  # Now lets read in our ZFS properties and reset them
  while read zLine
  do
      dSet="`echo $zLine | awk '{print $1}' | cut -d '@' -f 2- | cut -d '/' -f 2-`"
      prop="`echo $zLine | awk '{print $2}'`"
      val="`echo $zLine | awk '{print $3}'`"
      dChk="`echo $zLine | awk '{print $1}'`"

      # Remove header
      echo "$zLine" | grep -q PROPERTY
      if [ $? -eq 0 ] ; then continue ; fi

      # Don't need to set empty props
      if [ -z "$val" ] ; then continue ; fi

      # We can skip setting mountpoint on BEs
      echo $dSet | grep -q '^ROOT/'
      if [ $? -eq 0 -a "$prop" = "mountpoint" ] ; then continue; fi

      echo "$dChk" | grep -q '@'
      if [ $? -eq 0 ] ; then
        rc_halt "zfs set ${prop}=${val} ${ZPOOLNAME}@${dSet}"
        continue 
      fi
      echo "$dChk" | grep -q '/'
      if [ $? -eq 0 ] ; then
        rc_halt "zfs set ${prop}=${val} ${ZPOOLNAME}/${dSet}"
        continue 
      fi
      if [ "$dSet" = "$dChk" ] ; then
        if [ "$prop" = "mountpoint" ] ; then continue; fi
        rc_halt "zfs set ${prop}=${val} ${ZPOOLNAME}"
        continue 
      fi
  done < ${ZPFILE}

  # Lastly, lets set bootfs
  rc_halt "zpool set bootfs=${lastBE} ${ZPOOLNAME}"

}

restore_stamp_grub()
{
  rc_halt "cp ${TMPDIR}/zpool.cache /boot/zfs/zpool.cache"
  setup_grub
}

restore_umount_zfs()
{
   umount_all_dir "${FSMNT}"
}
