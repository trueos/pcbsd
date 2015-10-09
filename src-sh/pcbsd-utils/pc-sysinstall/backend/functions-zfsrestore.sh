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

do_zfs_restore_iscsi()
{
  load_iscsi_settings

  load_zpool_from_iscsi_file

  restore_zfs_from_local

  restore_boot_setup

  restore_umount_zfs
}

do_zfs_restore()
{
  load_ssh_settings

  restore_zfs_from_remote

  restore_boot_setup

  restore_umount_zfs
}

restore_boot_setup()
{
  rc_halt "cp ${TMPDIR}/zpool.cache /boot/zfs/zpool.cache"

  post_install_boot_setup
}

# Function to load Life-Preserver security file
load_iscsi_settings()
{
  get_value_from_cfg iscsiFile
  ISCSIFILE="$VAL"

  get_value_from_cfg iscsiPass
  ISCSIPASS="$VAL"
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
  # Get the ID of the pool
  GUID=$(zpool list -H -o guid ${ZPOOLNAME})
  rc_halt "zpool export ${ZPOOLNAME}"
  rc_halt "zpool import -N -R ${FSMNT} ${GUID}"

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

restore_umount_zfs()
{
   umount_all_dir "${FSMNT}"
}

connect_iscsi()
{
  # Check if stunnel is runing
  export spidFile="${TMPDIR}/.stask-restore"
  export STCFG="${TMPDIR}/.stcfg-restore"
  startSt=0;
  if [ -e "${spidFile}" ] ; then
     pgrep -F ${spidFile} >/dev/null 2>/dev/null
     if [ $? -eq 0 ] ; then startSt=1; fi
  fi

  # Time to start stunnel
  if [ "$startSt" != "1" ] ; then
     # Create the config
     echo "client = yes
foreground = yes
[iscsi]
accept=127.0.0.1:3260
connect = $REPHOST:$REPPORT" > ${STCFG}
     cat ${STCFG} >>${LOGOUT}
     # Start the client
     ( stunnel ${STCFG} >/dev/null 2>/dev/null )&
     echo "$!" > $spidFile
     sleep 1
  fi

  # Check if ISCSI is already init'd
  diskName=`iscsictl | grep "^${REPINAME}:${REPTARGET} " | grep "Connected:" | awk '{print $4}'`
  if [ -z "$diskName" ] ; then
     # Connect the ISCSI session
     echo "iscsictl -A -p 127.0.0.1 -t ${REPINAME}:$REPTARGET -u $REPUSER -s $REPPASS" >>${LOGOUT}
     iscsictl -A -p 127.0.0.1 -t ${REPINAME}:$REPTARGET -u $REPUSER -s $REPPASS >>${LOGOUT} 2>>${LOGOUT}
     if [ $? -ne 0 ] ; then return 1; fi
  fi

  # Now lets wait a reasonable ammount of time to see if iscsi becomes available
  i=0
  while :
  do
    # 60 seconds or so
    if [ "$i" = "12" ] ; then return 1; fi

    # Check if we have a connected target now
    diskName=`iscsictl | grep "^${REPINAME}:${REPTARGET} " | grep "Connected:" | awk '{print $4}'`
    if [ -n "$diskName" ] ; then break; fi

    i="`expr $i + 1`"
    sleep 5
  done

  echo "DISK: /dev/$diskName" >>${LOGOUT}
  sleep 5

  # Now lets confirm the iscsi target and prep
  if [ ! -e "/dev/$diskName" ] ; then echo "No such disk: $diskName" >>${LOGOUT} ; return 1; fi

  # Setup our variables for accessing the raw / encrypted disk
  export diskName
  export diskPart="${diskName}p1"
  export geliPart="${diskName}p1.eli"

  connect_geli_zpool "$1"
  return $?
}

connect_geli_zpool()
{
  # Make sure disk is partitioned
  gpart show $diskName >/dev/null 2>/dev/null
  if [ $? -ne 0 ] ; then
    exit_err "Failed to connect to iscsi host"
  fi

  # Make sure disk has GELI active on it, create if not
  if [ ! -e "/dev/${geliPart}" ] ; then
    rc_halt "geli attach -k ${GELIKEY} -p $diskPart"
  fi

  # Ok, make it through iscsi/geli, lets import the zpool
  zpool list $REPPOOL >/dev/null 2>/dev/null
  if [ $? -ne 0 ] ; then
    rc_halt "zpool import -o cachefile=none -N -f $REPPOOL"
  fi

  return 0
}

cleanup_iscsi()
{
  # All finished, lets export zpool
  zpool export $REPPOOL

  # Detach geli
  geli detach $diskName

  # Disconnect from ISCSI
  iscsictl -R -t ${REPINAME}:$REPTARGET

  # Kill the tunnel daemon
  kill -9 `cat $spidFile`
}

load_zpool_from_iscsi_file() {

  MD=`mdconfig -t vnode -f ${ISCSIFILE}`
  if [ -z "$ISCSIPASS" ] ; then
    exit_err "Missing GELI password file!"
  fi

  echo "Creating GELI provider..."
  cat ${ISCSIPASS} | geli attach -j - ${MD} >/dev/null 2>/dev/null
  if [ $? -ne 0 ] ; then
     mdconfig -d -u $MD
     exit_err "Failed GELI attach"
  fi
  rm ${ISCSIPASS}

  MNTDIR=`mktemp -d /tmp/XXXXXXXXXXXXXXXXXXX`
  mount /dev/${MD}.eli ${MNTDIR}
  if [ $? -ne 0 ] ; then
    geli stop /dev/${MD}.eli
    mdconfig -d -u $MD
    exit_err "Failed mounting"
  fi

  cp ${MNTDIR}/GELIKEY ${TMPDIR}/.GELIKEY
  GELIKEY="${TMPDIR}/.GELIKEY"

  # Read in the settings
  REPHOST=`cat ${MNTDIR}/REPHOST`
  REPUSER=`cat ${MNTDIR}/REPUSER`
  REPPORT=`cat ${MNTDIR}/REPPORT`
  REPRDATA=`cat ${MNTDIR}/REPRDATA`
  REPTARGET=`cat ${MNTDIR}/REPTARGET`
  REPPOOL=`cat ${MNTDIR}/REPPOOL`
  REPINAME=`cat ${MNTDIR}/REPINAME`
  REPPASS=`cat ${MNTDIR}/REPPASS`

  umount -f ${MNTDIR}
  rmdir ${MNTDIR}
  geli stop /dev/${MD}.eli
  mdconfig -d -u ${MD}

  echo "Connecting to iscsi / zpool... Please wait..."
  connect_iscsi
  if [ $? -ne 0 ] ; then
    cleanup_iscsi
    exit_err "Failed connecting to remote iscsi target!"
  fi

}

restore_zfs_from_local()
{

  # Get the local dataset to restore (One with /ROOT BE on it)
  ZFSDATASET="`zfs list -H -r ${REPPOOL} | awk '{print $1}' | grep '/ROOT$' | sed 's|/ROOT||g'`"

  lastSNAP=`zfs list -t snapshot -d 1 -H ${ZFSDATASET} | tail -1 | awk '{$1=$1}1' OFS=" " | cut -d '@' -f 2 | cut -d ' ' -f 1`

  echo "lastSnap: $lastSNAP"
  sleep 5
   
  # Lets start pulling our ZFS replication
  zSEND="zfs send -Rv ${ZFSDATASET}@${lastSNAP}"
  zRECV="zfs receive -evuF ${ZPOOLNAME}"
  $zSEND | $zRECV >/dev/null 2>/dev/null
  if [ $? -ne 0 ] ; then
     exit_err "Failed ZFS send / receive"
  fi

  # OK, with all the ZFS datasets received, we can now rename
  echo_log "Moving datasets to correct locations"
  for i in `zfs list -H -d 2 ${ZPOOLNAME} | awk '{print $1}'`
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
  GUID=$(zpool list -H -o guid ${ZPOOLNAME})
  rc_halt "zpool export ${ZPOOLNAME}"
  rc_halt "zpool import -N -R ${FSMNT} ${GUID}"

  # Lets mount the default dataset
  lastBE="`zfs list | grep ${ZPOOLNAME}/ROOT/ | tail -n 1 | awk '{print $1}'`"
  rc_halt "mount -t zfs ${lastBE} ${FSMNT}"

  # Lets get the list of properties to set
  zfs get -r -s local -o name,property,value all ${ZPOOLNAME} | grep lifepreserver-prop: > ${TMPDIR}/.zprops

  echo_log "Setting ZFS dataset properties.."
  # Now lets read in our ZFS properties and reset them
  while read zLine
  do
      dSet="`echo $zLine | awk '{print $1}'`"
      prop="`echo $zLine | awk '{print $2}' | sed 's|lifepreserver-prop:||d'`"
      val="`echo $zLine | awk '{print $3}'`"

      # Don't need to set empty props
      if [ -z "$val" ] ; then continue ; fi

      # We can skip setting mountpoint on BEs
      echo $dSet | grep -q '^ROOT/'
      if [ $? -eq 0 -a "$prop" = "mountpoint" ] ; then continue; fi

      echo "Setting prop: $dSet $prop = $val"

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
  done < ${TMPDIR}/.zprops
  rm ${TMPDIR}/.zprops

  # Lastly, lets set bootfs
  rc_halt "zpool set bootfs=${lastBE} ${ZPOOLNAME}"

}
