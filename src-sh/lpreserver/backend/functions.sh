#!/bin/sh
# Functions / variables for lpreserver
######################################################################
# DO NOT EDIT 

# Source external functions
. /usr/local/share/pcbsd/scripts/functions.sh

# Installation directory
PROGDIR="/usr/local/share/lpreserver"

# Location of settings 
DBDIR="/var/db/lpreserver"
if [ ! -d "$DBDIR" ] ; then mkdir -p ${DBDIR} ; fi

CMDLOG="${DBDIR}/lp-lastcmdout"
CMDLOG2="${DBDIR}/lp-lastcmdout2"
REPCONF="${DBDIR}/replication"
LOGDIR="/var/log/lpreserver"
REPLOGSEND="${LOGDIR}/lastrep-send-log"
REPLOGRECV="${LOGDIR}/lastrep-recv-log"
MSGQUEUE="${DBDIR}/.lpreserver.msg.$$"
export DBDIR LOGDIR PROGDIR CMDLOG REPCONF REPLOGSEND REPLOGRECV MSGQUEUE

# Create the logdir
if [ ! -d "$LOGDIR" ] ; then mkdir -p ${LOGDIR} ; fi

uname -r | grep -q 10.0
if [ $? -eq 0 ] ; then
  DEFAULT_ZPOOLFLAGS="async_destroy empty_bpobj lz4_compress multi_vdev_crash_dump"
else
  DEFAULT_ZPOOLFLAGS="async_destroy empty_bpobj filesystem_limits lz4_compress multi_vdev_crash_dump spacemap_histogram extensible_dataset bookmarks enabled_txg hole_birth"
fi

get_zpool_flags()
{
  ZPOOLFLAGS="-d"
  for i in $DEFAULT_ZPOOLFLAGS
  do
    ZPOOLFLAGS="$ZPOOLFLAGS -o feature@$i=enabled"
  done
}

#Set our Options
setOpts() {
  if [ -e "${DBDIR}/recursive-off" ] ; then
    export RECURMODE="OFF"
  else
    export RECURMODE="ON"
  fi

  if [ -e "${DBDIR}/emaillevel" ] ; then
    export EMAILMODE="`cat ${DBDIR}/emaillevel`"
  fi

  if [ -e "${DBDIR}/duwarn" ] ; then
    export DUWARN="`cat ${DBDIR}/duwarn`"
  else
    export DUWARN=70
  fi

  case $EMAILMODE in
      ALL|WARN|ERROR) ;;
	*) export EMAILMODE="WARN";;
  esac

  if [ -e "${DBDIR}/emails" ] ; then
    export EMAILADDY="`cat ${DBDIR}/emails`"
  fi

}
setOpts


# Check if a directory is mounted
isDirMounted() {
  mount | grep -q "on $1 ("
  return $?
}

mkZFSSnap() {
  if [ "$RECURMODE" = "ON" ] ; then
     flags="-r"
  else
     flags="-r"
  fi
  zdate=`date +%Y-%m-%d-%H-%M-%S`
  zfs snapshot $flags ${1}@$2${zdate} >${CMDLOG} 2>${CMDLOG}

  # Do we have a comment to set?
  if [ -n "$3" ] ; then
      zfs set lpreserver:comment="$3" ${1}@${2}${zdate}
  fi

  return $?
}

listZFSSnap() {
  echo "Snapshot				Comment"
  echo "-----------------------------------------------"
  for i in `zfs list -d 1 -t snapshot | grep -e "^${1}@" | awk '{print $1}'`
  do
     comment=`zfs get -o value lpreserver:comment $i | grep -v "VALUE"`
     echo "$i		$comment"
  done
}

rmZFSSnap() {
  `zfs list -d 1 -t snapshot | grep -q "^$1@$2 "` || return 1

  if [ "$RECURMODE" = "ON" ] ; then
     flags="-r"
  else
     flags="-r"
  fi
  zfs destroy -r ${1}@${2} >${CMDLOG} 2>${CMDLOG}
  return $?
}

revertZFSSnap() {
  # Make sure this is a valid snapshot
  `zfs list -d 1 -t snapshot | grep -q "^$1@$2 "` || exit_err "No such snapshot!"

  # Rollback the snapshot
  zfs rollback -R -f ${1}@$2
}

enable_cron_snap()
{
   cronscript="${PROGDIR}/backend/runsnap.sh"

   # Make sure we remove any old entries for this dataset
   cat /etc/crontab | grep -v " $cronscript $1" > /etc/crontab.new
   mv /etc/crontab.new /etc/crontab
   if [ "$2" = "OFF" ] ; then
      return 
   fi

   case $2 in
       daily) cLine="0       $4      *       *       *" ;;
      hourly) cLine="0       *       *       *       *" ;;
       30min) cLine="0,30    *       *       *       *" ;;
       10min) cLine="*/10    *       *       *       *" ;;
   5min|auto) cLine="*/5     *       *       *       *" ;;
           *) exit_err "Invalid time specified" ;;
   esac 

   echo -e "$cLine\troot    ${cronscript} $1 $3" >> /etc/crontab
}

enable_cron_scrub()
{
   cronscript="${PROGDIR}/backend/runscrub.sh"

   # Make sure we remove any old entries for this dataset
   cat /etc/crontab | grep -v " $cronscript $1" > /etc/crontab.new
   mv /etc/crontab.new /etc/crontab
   if [ "$2" = "OFF" ] ; then
      return 
   fi

   case $2 in
       daily) cLine="0       $3      *       *       *" ;;
       weekly) cLine="0       $4      *       *       $3" ;;
       monthly) cLine="0       $4      $3       *       *" ;;
           *) exit_err "Invalid time specified" ;;
   esac 

   echo -e "$cLine\troot    ${cronscript} $1" >> /etc/crontab
}

enable_watcher()
{
   cronscript="${PROGDIR}/backend/zfsmon.sh"

   # Check if the zfs monitor is already enabled
   grep -q " $cronscript" /etc/crontab
   if [ $? -eq 0 ] ; then return; fi

   cLine="*/30    *       *       *       *"

   echo -e "$cLine\troot    ${cronscript}" >> /etc/crontab
}

snaplist() {
  zfs list -d 1 -t snapshot | grep "^${1}@" | cut -d '@' -f 2 | awk '{print $1}'
}

echo_log() {
   echo "`date`: $@" >> ${LOGDIR}/lpreserver.log 
}

# E-Mail a message to the set addresses
# 1 = subject tag
# 2 = Message
email_msg() {
   if [ -z "$EMAILADDY" ] ; then return ; fi
   echo -e "$2"  | mail -s "$1 - `hostname`" $EMAILADDY
}

queue_msg() {
  echo -e "$1" >> ${MSGQUEUE}
  if [ -n "$2" ] ; then
    cat $2 >> ${MSGQUEUE}
  fi
}

echo_queue_msg() {
  if [ ! -e "$MSGQUEUE" ] ; then return ; fi
  cat ${MSGQUEUE}
  rm ${MSGQUEUE}
}

add_rep_task() {
  # add freenas.8343 backupuser 22 tank1/usr/home/kris tankbackup/backups sync
  HOST=$1
  USER=$2
  PORT=$3
  LDATA=$4
  RDATA=$5
  TIME=$6

  case $TIME in
     [0-9][0-9]|sync|hour|30min|10min|manual) ;;
     *) exit_err "Invalid time: $TIME"
  esac
 
  echo "Adding replication task for local dataset $LDATA"
  echo "----------------------------------------------------------"
  echo "   Remote Host: $HOST" 
  echo "   Remote User: $USER" 
  echo "   Remote Port: $PORT" 
  echo "Remote Dataset: $RDATA" 
  echo "          Time: $TIME" 
  echo "----------------------------------------------------------"
  echo "Don't forget to ensure that this user / dataset exists on the remote host"
  echo "with the correct permissions!"

  rem_rep_task "$LDATA" "$HOST"
  echo "$LDATA:$TIME:$HOST:$USER:$PORT:$RDATA" >> ${REPCONF}

  # If doing manual backups, stop here
  if [ "$TIME" = "manual" ] ; then return ; fi

  if [ "$TIME" != "sync" ] ; then
    case $TIME in
        hour) cTime="0     *" ;;
       30min) cTime="*/30     *" ;;
       10min) cTime="*/10     *" ;;
           *) cTime="0     $TIME" ;;
    esac
    cronscript="${PROGDIR}/backend/runrep.sh"
    cLine="$cTime       *       *       *"
    echo -e "$cLine\troot    ${cronscript} ${LDATA} ${HOST}" >> /etc/crontab
  fi
}

rem_rep_task() {
  if [ ! -e "$REPCONF" ] ; then return ; fi
  cat ${REPCONF} | grep -v "^${1}:.*:${2}:" > ${REPCONF}.tmp
  mv ${REPCONF}.tmp ${REPCONF}

  # Make sure we remove any old replication entries for this dataset
  cronscript="${PROGDIR}/backend/runrep.sh"
  cat /etc/crontab | grep -v " $cronscript ${1} ${2}" > /etc/crontab.new
  mv /etc/crontab.new /etc/crontab
}

list_rep_task() {
  if [ ! -e "$REPCONF" ] ; then return ; fi

  echo "Scheduled replications:"
  echo "---------------------------------"

  while read line
  do
     LDATA=`echo $line | cut -d ':' -f 1`
     TIME=`echo $line | cut -d ':' -f 2`
     HOST=`echo $line | cut -d ':' -f 3`
     USER=`echo $line | cut -d ':' -f 4`
     PORT=`echo $line | cut -d ':' -f 5`
     RDATA=`echo $line | cut -d ':' -f 6`

     echo "$LDATA -> $USER@$HOST[$PORT]:$RDATA Time: $TIME"

  done < ${REPCONF}
}

check_rep_task() {
  export DIDREP=0
  if [ ! -e "$REPCONF" ] ; then return 0; fi

  # Are we running as a sync task, or to a particular host?
  if [ "$2" = "sync" ] ; then
    repLine=`cat ${REPCONF} | grep "^${1}:${2}:" | head -n 1`
  else
    repLine=`cat ${REPCONF} | grep "^${1}:.*:${2}:"`
  fi

  if [ -z "$repLine" ] ; then return 0 ; fi

  # We have a replication task for this dataset, lets check if we need to do it now
  LDATA="$1"
  REPTIME=`echo $repLine | cut -d ':' -f 2`

  # Export the replication variables we will be using
  export REPHOST=`echo $repLine | cut -d ':' -f 3`
  export REPUSER=`echo $repLine | cut -d ':' -f 4`
  export REPPORT=`echo $repLine | cut -d ':' -f 5`
  export REPRDATA=`echo $repLine | cut -d ':' -f 6`

  # If we are checking for a sync task, and the rep isn't marked as sync we can continue
  if [ "$2" = "sync" -a "$REPTIME" != "sync" ] ; then continue; fi

  # Doing a replication task, check if one is in progress
  export pidFile="${DBDIR}/.reptask-`echo ${LDATA} | sed 's|/|-|g'`"
  if [ -e "${pidFile}" ] ; then
     pgrep -F ${pidFile} >/dev/null 2>/dev/null
     if [ $? -eq 0 ] ; then
        echo_log "Skipped replication on $LDATA, previous replication is still running."
        return 1
     else
        rm ${pidFile}
     fi
  fi

  # Save this PID
  echo "$$" > ${pidFile}

  # Is this a sync-task we do at the time of a snapshot?
  if [ "$2" = "sync" -a "$REPTIME" = "sync" ] ; then
     export DIDREP=1
     echo_log "Starting replication SYNC task on ${DATASET}: ${REPLOGSEND}"
     queue_msg "`date`: Starting replication SYNC task on ${DATASET}\n"
     start_rep_task "$LDATA"
     return $?
  else
     # Ready to do a scheduled replication
     export DIDREP=1
     echo_log "Starting replication SCHEDULED task on ${DATASET}: ${REPLOGSEND}"
     queue_msg "`date`: Starting replication SCHEDULED task on ${DATASET}\n"
     start_rep_task "$LDATA"
     return $?
  fi
}

load_iscsi_rep_data() {
  export REPHOST=`echo $repLine | cut -d ':' -f 3`
  export REPUSER=`echo $repLine | cut -d ':' -f 4`
  export REPPORT=`echo $repLine | cut -d ':' -f 5`
  export REPRDATA=`echo $repLine | cut -d ':' -f 6`
  export REPTARGET=`echo $repLine | cut -d ':' -f 7`
  export REPGELIKEY=`echo $repLine | cut -d ':' -f 8`
  export REPPOOL=`echo $repLine | cut -d ':' -f 9`
  export REPPASS=`echo $repLine | cut -d ':' -f 10-`
}

connect_iscsi() {

  # Check if stunnel is runing
  export spidFile="${DBDIR}/.stask-`echo ${LDATA} | sed 's|/|-|g'`"
  export STCFG="${DBDIR}/.stcfg-`echo ${LDATA} | sed 's|/|-|g'`"
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
accept=3260
connect = $REPHOST:$REPPORT" > ${STCFG}
     # Start the client
     ( stunnel ${STCFG} >>${CMDLOG} 2>>${CMDLOG} )&
     echo "$!" > $spidFile
  fi

  # Check if ISCSI is already init'd
  diskName=`iscsictl | grep "^iqn.2012-06.com.lpreserver:${REPTARGET} " | grep "Connected:" | awk '{print $4}'`
  if [ -z "$diskName" ] ; then
     # Connect the ISCSI session
     iscsictl -A -p 127.0.0.1 -t iqn.2012-06.com.lpreserver:$REPTARGET -u $REPUSER -s $REPPASS >>${CMDLOG} 2>>${CMDLOG}
     if [ $? -ne 0 ] ; then return 1; fi
  fi

  # Now lets wait a reasonable ammount of time to see if iscsi becomes available
  i=0
  while :
  do
    # 60 seconds or so
    if [ "$i" = "12" ] ; then return 1; fi

    # Check if we have a connected target now
    diskName=`iscsictl | grep "^iqn.2012-06.com.lpreserver:${REPTARGET} " | grep "Connected:" | awk '{print $4}'`
    if [ -n "$diskName" ] ; then break; fi

    i="`expr $i + 1`"
    sleep 5
  done

  # Now lets confirm the iscsi target and prep
  if [ ! -e "/dev/$diskName" ] ; then return 1; fi

  # Make sure disk is partitioned
  gpart show $diskName >/dev/null 2>/dev/null
  if [ $? -ne 0 ] ; then
    gpart create -s gpt $diskName >>$CMDLOG 2>>${CMDLOG}
    if [ $? -ne 0 ] ; then return 1; fi
  fi

  # Make sure disk has GELI active on it, create if not
  if [ ! -e "/dev/${diskName}.eli" ] ; then
    geli attach -k $REPGELIKEY -p $diskName >>$CMDLOG 2>>$CMDLOG
    if [ $? -ne 0 ] ; then
      # See if we can init this disk
      geli init -s 4096 -K $REPGELIKEY -P $diskName >>$CMDLOG 2>>$CMDLOG
      if [ $? -ne 0 ] ; then return 1; fi

      # Now try to attach again
      geli attach -k $REPGELIKEY -p $diskName >>$CMDLOG 2>>$CMDLOG
      if [ $? -ne 0 ] ; then return 1; fi
    fi
  fi

  # Ok, make it through iscsi/geli, lets import the zpool
  zpool list $REPPOOL >/dev/null 2>/dev/null
  if [ $? -ne 0 ] ; then
    zpool import -N -f $REPPOOL >>$CMDLOG 2>>$CMDLOG
    if [ $? -ne 0 ] ; then
      # No pool? Lets see if we can create
      get_zpool_flags
      zpool create $ZPOOLFLAGS -m none $REPPOOL ${diskName}.eli >>$CMDLOG 2>>$CMDLOG
      if [ $? -ne 0 ] ; then return 1; fi
    fi
  fi

  zpool list >>${CMDLOG} 2>>${CMDLOG}

  # Backups should now be ready to go
  return 0
}

cleanup_iscsi() {
  if [ "$REPRDATA" != "ISCSI" ] ; then return ; fi

  # All finished, lets export zpool
  zpool export $REPPOOL >>$CMDLOG 2>>$CMDLOG

  # Detach geli
  geli detach $diskName >>$CMDLOG 2>>$CMDLOG

  # Disconnect from ISCSI
  iscsictl -R -t iqn.2012-06.com.lpreserver:$REPTARGET >>$CMDLOG 2>>$CMDLOG

  # Kill the tunnel daemon
  kill -9 `cat $spidFile` >>$CMDLOG 2>>$CMDLOG
}


start_rep_task() {
  LDATA="$1"
  hName=`hostname`

  # If we are doing backup to ISCSI / encrypted target, load info now
  if [ "$REPRDATA" = "ISCSI" ] ; then
     ISCSI="true"
     load_iscsi_rep_data
     connect_iscsi
     if [ $? -ne 0 ] ; then
       FLOG=${LOGDIR}/lpreserver_failed.log
       echo "\nError Log:\n" >> ${FLOG}
       cleanup_iscsi >> ${FLOG}
       cat ${CMDLOG} >> ${FLOG}
       echo_log "FAILED replication task on ${DATASET}: LOGFILE: $FLOG"
       rm ${pidFile}
       return 1
     fi
  fi

  # Check for the last snapshot marked as replicated already
  lastSEND=`zfs get -d 1 lpreserver:${REPHOST} ${LDATA} | grep LATEST | awk '{$1=$1}1' OFS=" " | tail -1 | cut -d '@' -f 2 | cut -d ' ' -f 1`

  # Lets get the last snapshot for this dataset
  lastSNAP=`zfs list -t snapshot -d 1 -H ${LDATA} | tail -1 | awk '{$1=$1}1' OFS=" " | cut -d '@' -f 2 | cut -d ' ' -f 1`
 
  if [ "$lastSEND" = "$lastSNAP" ] ; then
     queue_msg "`date`: Last snapshot $lastSNAP is already marked as replicated!"
     rm ${pidFile}
     return 1
  fi

  # Starting replication, first lets check if we can do an incremental send
  if [ -n "$lastSEND" ] ; then
     zFLAGS="-Rv -I $lastSEND $LDATA@$lastSNAP"
  else
     zFLAGS="-Rv $LDATA@$lastSNAP"

     if [ -z "$ISCSI" ] ; then
       # This is a first-time replication, lets create the new target dataset
       ssh -p ${REPPORT} ${REPUSER}@${REPHOST} zfs create ${REPRDATA}/${hName} >${CMDLOG} 2>${CMDLOG}
     else
       zfs create ${REPPOOL}/${hName} >${CMDLOG} 2>${CMDLOG}
     fi
  fi

  if [ -z "$ISCSI" ] ; then
    zSEND="zfs send $zFLAGS"
    zRCV="ssh -p ${REPPORT} ${REPUSER}@${REPHOST} zfs receive -dvuF ${REPRDATA}/${hName}"
  else
    zSEND="zfs send $zFLAGS"
    zRCV="zfs receive -dvuF ${REPPOOL}/${hName}"
  fi

  queue_msg "Using ZFS send command:\n$zSEND | $zRCV\n\n"

  # Start up our process
  $zSEND 2>${REPLOGSEND} | $zRCV >${REPLOGRECV} 2>${REPLOGRECV}
  zStatus=$?
  queue_msg "ZFS SEND LOG:\n--------------\n" "${REPLOGSEND}"
  queue_msg "ZFS RCV LOG:\n--------------\n" "${REPLOGRECV}"

  cleanup_iscsi

  if [ $zStatus -eq 0 ] ; then
     # SUCCESS!
     # Lets mark our new latest snapshot and unmark the last one
     if [ -n "$lastSEND" ] ; then
       zfs set lpreserver:${REPHOST}=' ' ${LDATA}@$lastSEND
     fi
     zfs set lpreserver:${REPHOST}=LATEST ${LDATA}@$lastSNAP
     echo_log "Finished replication task on ${DATASET}"
     save_rep_props
     zStatus=$?
  else
     # FAILED :-(
     # Lets save the output for us to look at later
     FLOG=${LOGDIR}/lpreserver_failed.log
     echo "Failed with command:\n$zSEND | $zRCV\n" > ${FLOG}
     echo "\nSending log:\n" >> ${FLOG}
     cat ${REPLOGSEND} >> ${FLOG}
     echo "\nRecv log:\n" >> ${FLOG}
     cat ${REPLOGRECV} >> ${FLOG}
     echo_log "FAILED replication task on ${DATASET}: LOGFILE: $FLOG"
  fi

  rm ${pidFile}
  return $zStatus
}

save_rep_props() {
  # If we are not doing a recursive backup / complete dataset we can skip this
  if [ "$RECURMODE" != "ON" ] ; then return 0; fi
  if [ "`basename $DATASET`" != "$DATASET" ] ; then return 0; fi
  hName="`hostname`"

  echo_log "Saving dataset properties for: ${DATASET}"
  queue_msg "`date`: Saving dataset properties for: ${DATASET}\n"

  # Lets start by building a list of props to keep
  rProp=".lp-props-`echo ${REPRDATA}/${hName} | sed 's|/|#|g'`"

  zfs get -t filesystem -s local -r all $DATASET | awk '{$1=$1}1' OFS=" " | sed 's| local$||g' \
	| ssh -p ${REPPORT} ${REPUSER}@${REPHOST} "cat > \"$rProp\""
  if [ $? -eq 0 ] ; then
    echo_log "Successful save of dataset properties for: ${DATASET}"
    queue_msg "`date`: Successful save of dataset properties for: ${DATASET}\n"
    return 0
  else
    echo_log "Failed saving dataset properties for: ${DATASET}"
    queue_msg "`date`: Failed saving dataset properties for: ${DATASET}\n"
    return 1
  fi
}

listStatus() {

  for i in `grep "${PROGDIR}/backend/runsnap.sh" /etc/crontab | awk '{print $8}'`
  do
    echo -e "DATASET - TARGET - SNAPSHOT - REPLICATION"
    echo "---------------------------------------------------"

    REPHOST="NONE"
    lastSNAP="NONE"
    lastSEND="NONE"
    lastSNAP=`zfs list -t snapshot -d 1 -H ${i} | tail -1 | awk '{$1=$1}1' OFS=" " | cut -d '@' -f 2 | cut -d ' ' -f 1`

    cat ${REPCONF} | grep -q "^${i}:"
    if [ $? -ne 0 ] ; then
       echo "$i -> $REPHOST - $lastSNAP - $lastSEND"
       continue
    fi

    # Get some details about this host
    for repLine in `cat ${REPCONF} | grep "^${i}:"`
    do
      REPHOST=`echo $repLine | cut -d ':' -f 3`

      lastSEND=`zfs get -d 1 lpreserver:${REPHOST} ${i} | grep LATEST | awk '{$1=$1}1' OFS=" " | tail -1 | cut -d '@' -f 2 | cut -d ' ' -f 1`

      if [ -z "$lastSEND" ] ; then lastSEND="NONE"; fi
      if [ -z "$lastSNAP" ] ; then lastSNAP="NONE"; fi
      echo "$i -> $REPHOST - $lastSNAP - $lastSEND"
    done
  done
}

add_zpool_disk() {
   pool="$1"
   disk="$2"
   disk="`echo $disk | sed 's|/dev/||g'`"

   if [ -z "$pool" ] ; then
      exit_err "No pool specified"
      exit 0
   fi

   if [ -z "$disk" ] ; then
      exit_err "No disk specified"
      exit 0
   fi

   if [ ! -e "/dev/$disk" ] ; then
      exit_err "No such device: $disk"
      exit 0
   fi

   zpool list -H -v | awk '{print $1}' | grep -q "^$disk"
   if [ $? -eq 0 ] ; then
      exit_err "Error: This disk is already apart of a zpool!"
   fi

   # Check if pool exists
   zpool status $pool >/dev/null 2>/dev/null
   if [ $? -ne 0 ] ; then exit_err "Invalid pool: $pool"; fi

   # Cleanup the target disk
   echo "Deleting all partitions on: $disk"
   rc_nohalt "gpart destroy -F $disk" >/dev/null 2>/dev/null
   rc_nohalt "dd if=/dev/zero of=/dev/${disk} bs=1m count=1" >/dev/null 2>/dev/null
   rc_nohalt "dd if=/dev/zero of=/dev/${disk} bs=1m oseek=`diskinfo /dev/${disk} | awk '{print int($3 / (1024*1024)) - 4;}'`" >/dev/null 2>/dev/null

   # Grab the first disk in the pool
   mDisk=`zpool list -H -v | grep -v "^$pool" | awk '{print $1}' | grep -v "^mirror" | grep -v "^raidz" | head -n 1`

   # Is this MBR or GPT?
   echo $mDisk | grep -q 's[0-4][a-z]$'
   if [ $? -eq 0 ] ; then
      # MBR
      type="MBR"
      # Strip off the "a-z"
      rDiskDev=`echo $mDisk | rev | cut -c 2- | rev`
   else
      # GPT
      type="GPT"
      # Strip off the "p[1-9]"
      rDiskDev=`echo $mDisk | rev | cut -c 3- | rev`
   fi

   # Make sure this disk has a layout we can read
   gpart show $rDiskDev >/dev/null 2>/dev/null
   if [ $? -ne 0 ] ; then 
      exit_err "failed to get disk device layout $rDiskDev"
   fi

   # Get the size of "freebsd-zfs & freebsd-swap"
   sSize=`gpart show ${rDiskDev} | grep freebsd-swap | cut -d "(" -f 2 | cut -d ")" -f 1`
   zSize=`gpart show ${rDiskDev} | grep freebsd-zfs | cut -d "(" -f 2 | cut -d ")" -f 1`
   
   echo "Creating new partitions on $disk"
   if [ "$type" = "MBR" ] ; then
      # Create the new MBR layout
      rc_halt_s "gpart create -s MBR -f active $disk"
      rc_halt_s "gpart add -a 4k -t freebsd $disk"	
      rc_halt_s "gpart set -a active -i 1 $disk"
      rc_halt_s "gpart create -s BSD ${disk}s1"
      rc_halt_s "gpart add -t freebsd-zfs -s $zSize ${disk}s1"
      if [ -n "$sSize" ] ; then
        rc_halt_s "gpart add -t freebsd-swap -s $sSize ${disk}s1"
      fi
      aDev="${disk}s1a"
   else
      # Creating a GPT disk
      GRUBFLAGS="--modules='zfs part_gpt part_bsd geli'"
      rc_halt_s "gpart create -s GPT $disk"
      rc_halt_s "gpart add -b 34 -s 1M -t bios-boot $disk"
      rc_halt_s "gpart add -t freebsd-zfs -s $zSize ${disk}"
      if [ -n "$sSize" ] ; then
        rc_halt_s "gpart add -t freebsd-swap -s $sSize ${disk}"
      fi
      aDev="${disk}p2"
   fi

   # Now we can insert the target disk
   echo "Attaching to zpool: $aDev"
   rc_halt_s "zpool attach $pool $mDisk $aDev"

   # Lastly we need to stamp GRUB
   echo "Stamping GRUB on: $disk"
   rc_halt_s "grub-install $GRUBFLAGS --force /dev/${disk}"

   echo "Added $disk ($aDev) to zpool $pool. Resilver will begin automatically."
   exit 0
}

list_zpool_disks() {
   pool="$1"

   if [ -z "$pool" ] ; then
      exit_err "No pool specified"
      exit 0
   fi

   # Check if pool exists
   zpool status $pool >/dev/null 2>/dev/null
   if [ $? -ne 0 ] ; then exit_err "Invalid pool: $pool"; fi

   zpool list -H -v $pool
}

rem_zpool_disk() {
   pool="$1"
   disk="$2"

   if [ -z "$pool" ] ; then
      exit_err "No pool specified"
      exit 0
   fi

   if [ -z "$disk" ] ; then
      exit_err "No disk specified"
      exit 0
   fi

   # Check if pool exists
   zpool status $pool >/dev/null 2>/dev/null
   if [ $? -ne 0 ] ; then exit_err "Invalid pool: $pool"; fi

   zpool detach $pool $disk
   if [ $? -ne 0 ] ; then
      exit_err "Failed detaching $disk"
   fi 
   echo "$disk was detached successfully!"
   exit 0
}

offline_zpool_disk() {
   pool="$1"
   disk="$2"

   if [ -z "$pool" ] ; then
      exit_err "No pool specified"
      exit 0
   fi

   if [ -z "$disk" ] ; then
      exit_err "No disk specified"
      exit 0
   fi

   # Check if pool exists
   zpool status $pool >/dev/null 2>/dev/null
   if [ $? -ne 0 ] ; then exit_err "Invalid pool: $pool"; fi

   zpool offline $pool $disk
   exit $?
}

online_zpool_disk() {
   pool="$1"
   disk="$2"

   if [ -z "$pool" ] ; then
      exit_err "No pool specified"
      exit 0
   fi

   if [ -z "$disk" ] ; then
      exit_err "No disk specified"
      exit 0
   fi

   # Check if pool exists
   zpool status $pool >/dev/null 2>/dev/null
   if [ $? -ne 0 ] ; then exit_err "Invalid pool: $pool"; fi

   zpool online $pool $disk
   exit $?
}

init_rep_task() {

  LDATA="$1"
  if [ -z "$1" -o -z "$2" ] ; then
     exit_err "Usage: lpreserver replicate init <zpool> <target host>"
  fi

  repLine=`cat ${REPCONF} | grep "^${LDATA}:.*:${2}:"`
  if [ -z "$repLine" ] ; then exit_err "No such replication task: ${LDATA}";fi
 
  # We have a replication task for this set, get some vars
  hName=`hostname`
  REPHOST=`echo $repLine | cut -d ':' -f 3`
  REPUSER=`echo $repLine | cut -d ':' -f 4`
  REPPORT=`echo $repLine | cut -d ':' -f 5`
  REPRDATA=`echo $repLine | cut -d ':' -f 6`

  # First check if we even have a dataset on the remote
  ssh -p ${REPPORT} ${REPUSER}@${REPHOST} zfs list ${REPRDATA}/${hName} 2>/dev/null >/dev/null
  if [ $? -eq 0 ] ; then
     # Lets cleanup the remote side
     echo "Removing remote dataset: ${REPRDATA}/${hName}"
     ssh -p ${REPPORT} ${REPUSER}@${REPHOST} zfs destroy -r ${REPRDATA}/${hName}
     if [ $? -ne 0 ] ; then
        echo "Warning: Could not delete remote dataset ${REPRDATA}/${hName}"
     fi
  fi

  # Now lets mark none of our datasets as replicated
  lastSEND=`zfs get -d 1 lpreserver:${REPHOST} ${LDATA} | grep LATEST | awk '{$1=$1}1' OFS=" " | tail -1 | cut -d '@' -f 2 | cut -d ' ' -f 1`
  if [ -n "$lastSEND" ] ; then
     zfs set lpreserver:${REPHOST}=' ' ${LDATA}@$lastSEND
  fi

  echo "Ready to do full replication for: $LDATA"
}

## Function to remove the oldest life-preserver snapshot on the target
## zpool, used by zfsmon.sh when the disk space gets low
do_pool_cleanup()
{
  # Is this zpool managed by life-preserver?
  grep -q "${PROGDIR}/backend/runsnap.sh ${1} " /etc/crontab
  if [ $? -ne 0 ] ; then return ; fi

  # Before we start pruning, check if any replication is running
  local pidFile="${DBDIR}/.reptask-`echo ${1} | sed 's|/|-|g'`"
  if [ -e "${pidFile}" ] ; then
     pgrep -F ${pidFile} >/dev/null 2>/dev/null
     if [ $? -eq 0 ] ; then return; fi
  fi

  # Get the list of snapshots for this zpool
  snapList=$(snaplist "${1}")

  # Do any pruning
  for snap in $snapList
  do
     # Only remove snapshots which are auto-created by life-preserver
     cur="`echo $snap | cut -d '-' -f 1`"
     if [ "$cur" != "auto" ] ; then continue; fi

     echo_log "Pruning old snapshot: $snap"
     rmZFSSnap "${1}" "$snap"
     if [ $? -ne 0 ] ; then
       haveMsg=1
       echo_log "ERROR: (Low Disk Space) Failed pruning snapshot $snap on ${1}"
       queue_msg "ERROR: (Low Disk Space) Failed pruning snapshot $snap on ${1} @ `date` \n\r`cat $CMDLOG`"
     else
       queue_msg "(Low Disk Space) Auto-pruned snapshot: $snap on ${1} @ `date`\n\r`cat $CMDLOG`"
       haveMsg=1
     fi

     # We only prune a single snapshot at this time, so lets end
     break
  done

  return 0
}
