#!/bin/sh
# Functions / variables for lpreserver
######################################################################
# DO NOT EDIT
# Modified 7/14/2015 

# Source external functions
. /usr/local/share/pcbsd/scripts/functions.sh

PATH="${PATH}:/usr/local/bin:/usr/local/sbin"
export PATH

# Installation directory
PROGDIR="/usr/local/share/lpreserver"

# Location of settings 
DBDIR="/var/db/lpreserver"
DBDIREXCLUDES="/var/db/lpreserver/excludes"
DBDIRKEYS="/var/db/lpreserver/keys"
if [ ! -d "$DBDIR" ] ; then mkdir -p ${DBDIR}
fi
if [ ! -d "$DBDIREXCLUDES" ] ; then mkdir -p ${DBDIREXCLUDES}
fi
if [ ! -d "$DBDIRKEYS" ] ; then mkdir -p ${DBDIRKEYS}
fi

CMDLOG="${DBDIR}/lp-lastcmdout"
STUNNELLOG="${LOGDIR}/lp-stunnel"
ISCSILOG="${LOGDIR}/lp-iscsi"
CMDLOG2="${DBDIR}/lp-lastcmdout2"
REPCONF="${DBDIR}/replication"
LOGDIR="/var/log/lpreserver"
REPLOGSEND="${LOGDIR}/lastrep-send-log"
REPLOGRECV="${LOGDIR}/lastrep-recv-log"
MSGQUEUE="${DBDIR}/.lpreserver.msg.$$"
export DBDIR LOGDIR PROGDIR CMDLOG REPCONF REPLOGSEND REPLOGRECV MSGQUEUE

# Create the logdir
if [ ! -d "$LOGDIR" ] ; then 
   mkdir -p ${LOGDIR}
fi

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
  zdate=`date +%Y-%m-%d-%H-%M-%S`
  if [ "$RECURMODE" = "ON" ] ; then
    # Get list of datasets to snap
    build_dset_list "$1" "snap"
    unset _snap
    for dset in $DSETS
    do
      _snap="${_snap} ${dset}@${2}${zdate}"
    done
    echo "zfs snapshot ${_snap}" > /tmp/.zSnap.$$
    sh /tmp/.zSnap.$$
    err=$?
    rm /tmp/.zSnap.$$
  else
    flags=""
    zfs snapshot $flags ${1}@${2}${zdate} >>${CMDLOG} 2>>${CMDLOG}
    err=$?
  fi

  # Do we have a comment to set?
  if [ -n "$3" ] ; then
    zfs set lpreserver:comment="${3}" ${1}@${2}${zdate}
  fi

  return $err
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
  `zfs list -d 1 -t snapshot $1 | grep -q "^$1@$2 "` || return 1

  zfs destroy -R ${1}@${2} >${CMDLOG} 2>${CMDLOG}
  return $?
}

revertZFSSnap() {
  # Make sure this is a valid snapshot
  `zfs list -d 1 -t snapshot $1 | grep -q "^$1@$2 "` || exit_err "No such snapshot!"

  # Rollback the snapshot
  zfs rollback -R -f ${1}@$2
  return $?
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
   cat /etc/crontab | grep -v "$cronscript $1" > /etc/crontab.new
   mv /etc/crontab.new /etc/crontab
   
   cat /usr/local/etc/anacrontab | grep -v "$cronscript $1" > /usr/local/etc/anacrontab.new
   mv /usr/local/etc/anacrontab.new /usr/local/etc/anacrontab
   
   if [ "$2" = "OFF" ] ; then
      return 
   fi

   case $2 in
       daily) cLine="0       $3      *       *       *" ;;
       weekly) cLine="0       $4      *       *       $3" ;;
       monthly) cLine="0       $4      $3       *       *" ;;
       anacron) cLine="$3\t60\tautoscrub";;
           *) exit_err "Invalid time specified" ;;
   esac 

   if [ "$2" = "anacron" ]; then
      echo -e "$cLine\t${cronscript} $1" >> /usr/local/etc/anacrontab
   else
     echo -e "$cLine\troot    ${cronscript} $1" >> /etc/crontab
   fi
}

list_cron_snap()
{
echo "Datasets scheduled for snapshots:"
echo "---------------------------------"
for i in `grep "${PROGDIR}/backend/runsnap.sh" /etc/crontab | awk '{print $8}'`
do
   min=`grep "${PROGDIR}/backend/runsnap.sh ${i}" /etc/crontab | awk '{print $1}'`
   hour=`grep "${PROGDIR}/backend/runsnap.sh ${i}" /etc/crontab | awk '{print $2}'`
   count=`grep "${PROGDIR}/backend/runsnap.sh ${i}" /etc/crontab | awk '{print $9}'`
   time="Min: $min Hour: $hour";
   if [ "$min" = "0" -a "$hour" != '*' ] ; then time="daily@$hour" ; fi
   if [ "$min" = "0" -a "$hour" = '*' ] ; then time="hourly" ; fi
   if [ "$min" = "0,30" ] ; then time="30min" ; fi
   if [ "$min" = '*/10' ] ; then time="10min" ; fi
   if [ "$min" = '*/5' ] ; then time="5min" ; fi
   if [ "$min" = '*/5' -a "$count" = "auto" ] ; then time="Automatic" ; fi
   echo "$i - $time - total: $count"
   echo ""
done
}

list_cron_scrub()
{
echo "Pools scheduled for scrub:"
echo "---------------------------------"
for i in `grep "${PROGDIR}/backend/runscrub.sh" /etc/crontab | awk '{print $8}'`
do
   hour=`grep "${PROGDIR}/backend/runscrub.sh ${i}" /etc/crontab | awk '{print $2}'`
   day_week=`grep "${PROGDIR}/backend/runscrub.sh ${i}" /etc/crontab | awk '{print $5}'`
   day_month=`grep "${PROGDIR}/backend/runscrub.sh ${i}" /etc/crontab | awk '{print $3}'`
   if [ "$hour" != '*' -a "$day_week" = '*' -a "$day_month" = '*' ] ; then time="daily @ $hour" ; fi
   if [ "$day_week" != '*' ] ; then
      time="weekly @ $day_week @ $hour"
   fi
   if [ "$day_month" != '*' ] ; then
      time="monthly @ $day_month @ $hour"               
   fi
   echo "$i - $time"
   echo ""
done

for i in `grep "${PROGDIR}/backend/runscrub.sh" /usr/local/etc/anacrontab | awk '{print $5}'`
do
   time=`grep "${PROGDIR}/backend/runscrub.sh ${i}" /usr/local/etc/anacrontab | awk '{print $1}'`
   echo "$i - every $time days"
   echo ""
done

}

enable_watcher()
{
   cronscript="${PROGDIR}/backend/zfsmon.sh"

   # Check if the zfs monitor is already enabled
   grep -q " $cronscript" /etc/crontab
   if [ $? -eq 0 ] ; then
      return
   fi

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
  if [ ! -e "$MSGQUEUE" ] ; then
     return
  fi
  cat ${MSGQUEUE}
  rm ${MSGQUEUE}
}

read_lps_file()
{
  unset HOST
  unset USER
  unset PASS
  unset ITARGET

  # Read in the LPS file
  HOST="`grep '^ihost:' $1 | cut -d ':' -f 2 | sed 's| ||g'`"
  USER="`grep '^iuser:' $1 | cut -d ':' -f 2 | sed 's| ||g'`"
  PASS="`grep '^ipassword:' $1 | cut -d ':' -f 2 | sed 's| ||g'`"
  ITARGET="`grep '^itarget:' $LPS | cut -d ':' -f 2 | sed 's| ||g'`"
  if [ -z "$HOST" ] ; then
     exit_err "Missing ihost! Invalid LPS file?"
  fi
  if [ -z "$USER" ] ; then
     exit_err "Missing iuser! Invalid LPS file?"
  fi
  if [ -z "$PASS" ] ; then
     exit_err "Missing ipass! Invalid LPS file?"
  fi
  if [ -z "$ITARGET" ] ; then
     exit_err "Missing itarget! Invalid LPS file?"
  fi
}

add_rep_from_iscsi_file() {
  LPFILE="$1"
  LDATA="$2"
  TIME="$3"
  PASSFILE="$4"

  MD=`mdconfig -t vnode -f ${LPFILE}`
  if [ -n "$PASSFILE" ] ; then
    echo "Creating GELI provider..."
    cat ${PASSFILE} | geli attach -j - ${MD} >/dev/null 2>/dev/null
    if [ $? -ne 0 ] ; then
       mdconfig -d -u $MD
       exit_err "Failed GELI attach"
    fi
    rm ${PASSFILE}
  else
    echo "Please enter the password for this backup iscsi file:"
    echo -e ">\c"
    stty -echo
    read PASSWORD
    stty echo
    printf "\n"
    echo "$PASSWORD" | geli attach -j - ${MD} >/dev/null 2>/dev/null
    if [ $? -ne 0 ] ; then
       mdconfig -d -u $MD
       exit_err "Failed GELI attach"
    fi
  fi

  MNTDIR=`mktemp -d /tmp/XXXXXXXXXXXXXXXXXXX`
  mount /dev/${MD}.eli ${MNTDIR}
  if [ $? -ne 0 ] ; then
    geli stop /dev/${MD}.eli
    mdconfig -d -u $MD
    exit_err "Failed mounting"
  fi
  GELIKEY="${MNTDIR}/GELIKEY"

  # Read in the settings
  REPHOST=`cat ${MNTDIR}/REPHOST`
  REPUSER=`cat ${MNTDIR}/REPUSER`
  REPPORT=`cat ${MNTDIR}/REPPORT`
  REPRDATA=`cat ${MNTDIR}/REPRDATA`
  REPTARGET=`cat ${MNTDIR}/REPTARGET`
  REPPOOL=`cat ${MNTDIR}/REPPOOL`
  REPINAME=`cat ${MNTDIR}/REPINAME`
  REPPASS=`cat ${MNTDIR}/REPPASS`

  if [ ! -e "$GELIKEY" ] ; then
     exit_err "Missing GELI key: $GELIKEY"
  fi
  LGELIKEY="${DBDIR}/keys/`echo ${HOST}${USER}${LDATA} | sha256 -q`.key"
  cp $GELIKEY $LGELIKEY
  chmod 600 ${LGELIKEY}

  rem_rep_task "$LDATA" "$REPHOST"
  echo "${LDATA}:${TIME}:${REPHOST}:${REPUSER}:${REPPORT}:ISCSI:${REPTARGET}:${LGELIKEY}:${REPPOOL}:${REPINAME}:${REPPASS}" >> ${REPCONF}

  umount -f ${MNTDIR}
  rmdir ${MNTDIR}
  geli stop /dev/${MD}.eli
  mdconfig -d -u ${MD}

  if [ -e ${CMDLOG} ] ; then rm ${CMDLOG}; fi

  # Lets test connecting to the iscsi target
  repLine=`cat ${REPCONF} | grep "^${LDATA}:.*:${REPHOST}:"`
  load_iscsi_rep_data
  echo "Running test iscsi / zpool connection... Please wait..."
  connect_iscsi
  if [ $? -ne 0 ] ; then
    cleanup_iscsi
    rm ${LGELIKEY}
    rem_rep_task "$LDATA" "$HOST"
    exit_err "Failed connecting to / importing of remote iscsi target!"
  fi
  cleanup_iscsi

  finish_add_iscsi_target
}

add_rep_iscsi_task() {
  LPS="$1"
  LDATA="$2"
  TIME="$3"
  GELIKEY="$4"

  case $TIME in
     0|[1-9]|1[0-9]|2[0-3]|sync|hour|30min|10min|manual) ;;
     *) exit_err "Invalid time: $TIME"
  esac

  # Make the GELI key dir
  if [ ! -d "${DBDIR}/keys" ] ; then
    mkdir -p ${DBDIR}/keys
  fi

  # See if this file is a LPS or LPISCSI file
  file ${LPS} | grep -q data
  if [ $? -eq 0 ] ; then
     add_rep_from_iscsi_file "$LPS" "$2" "$3" "$4"
     exit 0
  fi

  # Read data from the LPS file
  read_lps_file "$LPS"
  RZPOOL="lp-$USER-backup"

  # Setup the GELI key
  LGELIKEY="${DBDIR}/keys/`echo ${HOST}${USER}${LDATA} | sha256 -q`.key"
  if [ -n "$GELIKEY" ] ; then
    if [ ! -e "$GELIKEY" ] ; then
       exit_err "Missing GELI key: $GELIKEY"
    fi
    cp $GELIKEY $LGELIKEY
  else
    openssl genrsa 2048 2>/dev/null | grep -v "^----" > ${LGELIKEY}
  fi
  chmod 600 ${LGELIKEY}

  rem_rep_task "$LDATA" "$HOST"
  echo "${LDATA}:${TIME}:${HOST}:${USER}:9555:ISCSI:${ITARGET}:${LGELIKEY}:${RZPOOL}:iqn.2012-06.com.lpreserver:${PASS}" >> ${REPCONF}

  if [ -e ${CMDLOG} ] ; then
     rm ${CMDLOG}
  fi
  # Lets test connecting to the iscsi target
  repLine=`cat ${REPCONF} | grep "^${LDATA}:.*:${HOST}:"`
  load_iscsi_rep_data
  echo "Running initial iscsi / zpool init... Please wait..."
  connect_iscsi "init"
  if [ $? -ne 0 ] ; then
    cat ${CMDLOG} 2>/dev/null
    cleanup_iscsi
    rm ${LGELIKEY}
    rem_rep_task "$LDATA" "$HOST"
    exit_err "Failed connecting / init of remote iscsi target!"
  fi
  cleanup_iscsi

  finish_add_iscsi_target
}

finish_add_iscsi_target()
{

  if [ "$TIME" != "sync" -a "$TIME" != "manual" ] ; then
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

  echo "Adding iSCSI replication task for local dataset $LDATA"
  echo "----------------------------------------------------------"
  echo "   Remote Host: $HOST"
  echo "   Remote User: $USER"
  echo "  Remote zpool: $RZPOOL"
  echo "          Time: $TIME"
  echo "----------------------------------------------------------"
  echo ""
  echo "!! - WARNING - !!"
  echo "This is an ENCRYPTED backup!"
  echo "If you lose your encryption key, your data WILL be lost"
  echo "Please backup the following key in a safe location"
  echo ""
  echo "Key: ${LGELIKEY}"
  echo ""
  cat ${LGELIKEY}
  exit 0
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
     0|[1-9]|1[0-9]|2[0-3]|sync|hour|30min|10min|manual) ;;
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
  if [ ! -e "$REPCONF" ] ; then
     return 0
  fi

  # Are we running as a sync task, or to a particular host?
  if [ "$2" = "sync" ] ; then
    repLine=`cat ${REPCONF} | grep "^${1}:${2}:" | head -n 1`
  else
    repLine=`cat ${REPCONF} | grep "^${1}:.*:${2}:"`
  fi

  if [ -z "$repLine" ] ; then
     return 0
  fi

  # We have a replication task for this dataset, lets check if we need to do it now
  LDATA="$1"
  REPTIME=`echo $repLine | cut -d ':' -f 2`

  # Export the replication variables we will be using
  export REPHOST=`echo $repLine | cut -d ':' -f 3`
  export REPUSER=`echo $repLine | cut -d ':' -f 4`
  export REPPORT=`echo $repLine | cut -d ':' -f 5`
  export REPRDATA=`echo $repLine | cut -d ':' -f 6`

  # If we are checking for a sync task, and the rep isn't marked as sync we can continue
  if [ "$2" = "sync" -a "$REPTIME" != "sync" ] ; then
     continue
  fi

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
  export REPINAME=`echo $repLine | cut -d ':' -f 10`
  export REPPASS=`echo $repLine | cut -d ':' -f 11-`
}

# $1 = connectonly / init
connect_iscsi() {

  # Set if we just started iscsi
  export startISCSI="0";

  # Check if stunnel is runing
  export spidFile="${DBDIR}/.stask-`echo ${LDATA} | sed 's|/|-|g'`"
  export STCFG="${DBDIR}/.stcfg-`echo ${LDATA} | sed 's|/|-|g'`"
  startSt=0
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
     cat ${STCFG} >> ${STUNNELLOG}
     # Start the client
     ( stunnel ${STCFG} >>${STUNNELLOG} 2>>${STUNNELLOG} )&
     echo "$!" > $spidFile
     sleep 1
  fi

  # Check if iSCSI is already init'd
  diskName=`iscsictl | grep "^${REPINAME}:${REPTARGET} " | grep "Connected:" | awk '{print $4}'`
  if [ -z "$diskName" ] ; then
     startISCSI=1
     # Connect the iSCSI session
     echo "iscsictl -A -p 127.0.0.1 -t ${REPINAME}:$REPTARGET -u $REPUSER -s $REPPASS" >${ISCSILOG}
     iscsictl -A -p 127.0.0.1 -t ${REPINAME}:$REPTARGET -u $REPUSER -s $REPPASS >>${ISCSILOG} 2>>${ISCSILOG}
     if [ $? -ne 0 ] ; then 
        return 1
     fi
  fi

  # Now lets wait a reasonable ammount of time to see if iscsi becomes available
  i=0
  while :
  do
    # 60 seconds or so
    if [ "$i" = "12" ] ; then
      iscsictl >> ${ISCSILOG} 2>>${ISCSILOG}
      echo "Failed connecting to iscsi in 60 seconds..." >> ${ISCSILOG}
      return 1;
    fi

    # Check if we have a connected target now
    diskName=`iscsictl | grep "^${REPINAME}:${REPTARGET} " | grep "Connected:" | awk '{print $4}'`
    if [ -n "$diskName" ] ; then
       break
    fi
    i="`expr $i + 1`"
    sleep 5
  done

  echo "DISK: /dev/$diskName" >>${CMDLOG}
  sleep 5

  # Now lets confirm the iscsi target and prep
  if [ -z "$diskName" -o ! -e "/dev/$diskName" ] ; then echo "No such disk: $diskName" >>${CMDLOG} ; return 1; fi

  # Setup our variables for accessing the raw / encrypted disk
  export diskName
  export diskPart="${diskName}p1"
  export geliPart="${diskName}p1.eli"

  if [ "$1" != "connectonly" ] ; then
    connect_geli_zpool "$1"
    return $?
  fi
}

# $1 = {null}/init
connect_geli_zpool()
{
  # Make sure disk is partitioned
  gpart show $diskName >/dev/null 2>/dev/null
  if [ $? -ne 0 ] ; then
    if [ "$1" != "init" ] ; then
       return 1
    fi
    gpart create -s gpt $diskName >>$CMDLOG 2>>${CMDLOG}
    if [ $? -ne 0 ] ; then return 1; fi
    if [ ! -e "/dev/${diskPart}" ] ; then
      gpart add -t freebsd-zfs $diskName >>$CMDLOG 2>>${CMDLOG}
      if [ $? -ne 0 ] ; then 
      return 1
      fi
    fi
    sleep 5
  fi


  # Make sure disk has GELI active on it, create if not
  if [ ! -e "/dev/${geliPart}" ] ; then
    geli attach -k $REPGELIKEY -p $diskPart >>$CMDLOG 2>>$CMDLOG
    if [ $? -ne 0 ] ; then
      if [ "$1" != "init" ] ; then
         return 1
      fi
      sleep 5

      # See if we can init this disk
      geli init -s 4096 -K $REPGELIKEY -P $diskPart >>$CMDLOG 2>>$CMDLOG
      if [ $? -ne 0 ] ; then
         echo "Failed to init disk: $diskPart" >>$CMDLOG
         return 1
      fi

      # Now try to attach again
      geli attach -k $REPGELIKEY -p $diskPart >>$CMDLOG 2>>$CMDLOG
      if [ $? -ne 0 ] ; then echo "Failed to attach geli disk" >> ${CMDLOG} ; return 1; fi

      sleep 5
    fi
  fi

  # Ok, make it through iscsi/geli, lets import the zpool
  zpool list $REPPOOL >/dev/null 2>/dev/null
  if [ $? -ne 0 ] ; then
    zpool import -o cachefile=none -N -f $REPPOOL >>$CMDLOG 2>>$CMDLOG
    if [ $? -ne 0 ] ; then
      if [ "$1" != "init" ] ; then
         return 1
      fi
      # No pool? Lets see if we can create
      get_zpool_flags
      # Make sure the new zpool uses 4k sector size
      sysctl vfs.zfs.min_auto_ashift=12 >/dev/null 2>/dev/null
      zpool create $ZPOOLFLAGS -m none $REPPOOL ${geliPart} >>$CMDLOG 2>>$CMDLOG
      if [ $? -ne 0 ] ; then
         echo "Failed creating pool: $geliPart" >> ${CMDLOG}
         return 1
      fi
      sleep 2
    fi
  fi

  # Backups should now be ready to go
  return 0
}

cleanup_iscsi() {
  if [ "$REPRDATA" != "ISCSI" ] ; then
     return
  fi

  if [ "$startISCSI" = "0" ] ; then
     return 0
  fi

  # All finished, lets export zpool
  zpool export $REPPOOL >>$CMDLOG 2>>$CMDLOG

  # Detach geli
  geli detach $diskName >>$CMDLOG 2>>$CMDLOG

  # Disconnect from ISCSI
  iscsictl -R -t ${REPINAME}:$REPTARGET >>$CMDLOG 2>>$CMDLOG

  # Kill the tunnel daemon
  kill -9 `cat $spidFile` >>$CMDLOG 2>>$CMDLOG
}

# After replication of a dataset, we need to save local properties of that set
save_remote_props() {
  local dset="$1"

  # Get list of local props for this dataset
  zfs get -H -s local -o property,value all ${dset} | tr '\t' ' ' > /tmp/.dProps.$$

  # Now set these as user-props on the remote side
  while read propline
  do
    pname="`echo $propline | cut -d ' ' -f 1`"
    pval="`echo $propline | cut -d ' ' -f 2-`"
    if [ -z "$pname" -o "$pval" = "-" ] ; then
       continue
    fi
    rdset=""
    if [ "$dset" != "$LDATA" ] ; then
      # Strip off the zpool/top level name
      rdset=`echo $dset | sed "s|^${LDATA}||g"`
    fi

    # Set the property now
    ${CMDPREFIX} zfs set lifepreserver-prop:${pname}="${pval}" ${REMOTEDSET}/${hName}${rdset}
    if [ $? -ne 0 ] ; then
      echo "WARNING: Failed to set lifepreserver-prop:$pname=$pval property on ${dset}"
      echo_log "WARNING: Failed to set lifepreserver-prop:$pname=$pval property on ${dset}"
      queue_msg "`date`: Failed to set lifepreserver-prop:$pname=$pval property on ${dset}\n"
    fi
  done < /tmp/.dProps.$$
  rm /tmp/.dProps.$$ 2>/dev/null
}

# Build list of datasets
# $1 = Base zpool / dset
# $2 = {snap|rep} - Exclude list to use
build_dset_list()
{
  if [ -z "$1" -o -z "$2" ] ; then
    exit_err "build_dset_list() - Internal usage error!"
  fi

  EXCLFILE="${DBDIREXCLUDES}/`echo ${1} | sed 's|/|-|g'`-${2}"

  DSETS=""
  # If no exclude file, just return full list
  if [ ! -e "$EXCLFILE" ] ; then
    # Lets traverse the tree, build list of datasets to snap
    IFS=$'\n';
    for i in `zfs list -H -r -o name ${1}`
    do
      DSETS="$DSETS ${i}"
    done
    unset IFS
    sort_dset_by_clone
    return 0
  else
    # We have excludes, lets prune those
    zfs list -H -r -o name ${1} >/tmp/.dSet.$$
    while read line
    do
      cat /tmp/.dSet.$$ | grep -v "^$line" > /tmp/.dSet-new.$$
      mv /tmp/.dSet-new.$$ /tmp/.dSet.$$
    done < ${EXCLFILE}

    # Now build variable
    while read i
    do
      DSETS="$DSETS ${i}"
    done < /tmp/.dSet.$$

    # Cleanup and return
    rm /tmp/.dSet.$$
    sort_dset_by_clone
    return 0
  fi
}

sort_dset_by_clone() {
  local PSETS=""
  local CSETS=""
  for dset in $DSETS
  do
     origin=`zfs list -H -o origin $dset`
     # If no ORIGIN, we can add it back to the list
     if [ -z "$origin" -o "$origin" = "-" ] ; then
        PSETS=" $PSETS $dset "
        continue
     fi
     CSETS=" $CSETS $dset "
  done

  DSETS="$PSETS"
  if [ -n "$CSETS" ] ; then
     add_dset_clone "$CSETS"
  fi
}

add_dset_clone() {

  local CSETS=""
  unset PASSDSETS
  local PASSDSETS=""

  for dset in ${1}
  do
     origin=`zfs list -H -o origin $dset`
     originraw="`echo $origin | cut -d '@' -f 1`"
     case "${DSETS}" in
        *" $originraw "*) DSETS=" $DSETS $dset " ;;
        *) PASSDSETS=" $PASSDSETS $dset ";;
     esac
  done
  # Be recursive until we add them all
  if [ -n "$PASSDSETS" ] ; then
     add_dset_clone "$PASSDSETS"
  fi
}

prune_remote_datasets() {
  # Remove any remote datasets which no longer exist for replication here
  while read rdline
  do
    if [ -z "${rdline}" ] ; then
       continue
    fi
    noprune=0
    rdset="`echo ${rdline} | tr -s '\t' ' ' | cut -d ' ' -f 1`"
    if [ "$rdset" = "-" ] ; then
       continue
    fi

    for dset in ${DSETS}
    do
      dcmp="/`echo $dset | cut -d '/' -f 2-`"
      if [ "$rdset" = "$dcmp" ] ; then
         noprune=1
         break
      fi
    done

    if [ $noprune -eq 0 ] ; then
     echo_log "Removing ${REMOTEDSET}/${hName}${rdset} - No longer exists or excluded on host"
     queue_msg "`date`: Removing ${REMOTEDSET}/${hName}${rdset} - No longer exists or excluded on host"
     ${CMDPREFIX} zfs destroy -R ${REMOTEDSET}/${hName}${rdset}
     if [ $? -ne 0 ] ; then
       queue_msg "`date`: FAILED Removing ${REMOTEDSET}/${hName}${rdset}"
     fi
    fi
  done < ${1}
}

prune_old_remote_snaps() {
  lSnaps="/tmp/.lp-lsnaps.$$"

  zfs list -H -t snapshot -d 1 ${1} | awk '{print $1}' | cut -d '@' -f 2- >${lSnaps}
  sync

  for rsnap in `${CMDPREFIX} zfs list -H -t snapshot -d 1 ${2} | awk '{print $1}' | cut -d '@' -f 2-`
  do
    grep -q "^${rsnap}\$" ${lSnaps}
    if [ $? -eq 0 ]; then continue; fi
    
    queue_msg "`date`: Removing old snapshot ${2}@${rsnap}"
    ${CMDPREFIX} zfs destroy -R ${2}@${rsnap}
    if [ $? -ne 0 ] ; then
      echo_log "FAILED Removing old snapshot ${2}@${rsnap}"
      queue_msg "`date`: FAILED Removing old snapshot ${2}@${rsnap}"
    fi
  done

  rm ${lSnaps}
}

start_rep_task() {
  LDATA="$1"
  hName=`hostname`

  #save_mount_props "${DATASET}"

  # Set the remote dataset we are targeting
  local REMOTEDSET="${REPRDATA}"

  # If we are doing backup to ISCSI / encrypted target, load info now
  if [ "$REPRDATA" = "ISCSI" ] ; then
     ISCSI="true"
     load_iscsi_rep_data
     REMOTEDSET="${REPPOOL}"
     connect_iscsi
     if [ $? -ne 0 ] ; then
       FLOG=${LOGDIR}/lpreserver_failed.log
       echo "\nError Log:\n" >> ${FLOG}
       cleanup_iscsi >> ${FLOG}
       cat ${CMDLOG} >> ${FLOG}
       echo_log "FAILED replication task on ${DATASET} -> ${REPHOST}: LOGFILE: $FLOG"
       queue_msg "FAILED replication task on ${DATASET} -> ${REPHOST}: LOGFILE: $FLOG"
       queue_msg "`cat ${FLOG}`"
       rm ${pidFile}
       return 1
     fi
  fi

  # If we are doing SSH backup, set a prefix to remote commands
  if [ -z "$ISCSI" ] ; then
    CMDPREFIX="ssh -p ${REPPORT} ${REPUSER}@${REPHOST}"
  else
    CMDPREFIX=""
  fi

  # Check if the remote dataset exists, bail if not
  ${CMDPREFIX} zfs list -d 1 ${REMOTEDSET} >/dev/null 2>/dev/null
  if [ $? -ne 0 ] ; then
    echo_log "Cannot locate remote dataset ${REMOTEDSET}: LOGFILE: $FLOG"
    queue_msg "Cannot locate remote dataset ${REMOTEDSET}: LOGFILE: $FLOG"
    queue_msg "`cat ${FLOG}`"
    rm ${pidFile}
    return 1
  fi

  # Check for the last snapshot marked as replicated already
  lastSEND=`zfs get -d 1 lpreserver:${REPHOST} ${LDATA} | grep LATEST | awk '{$1=$1}1' OFS=" " | tail -1 | cut -d '@' -f 2 | cut -d ' ' -f 1`

  # Lets get the last snapshot for this dataset
  lastSNAP=`zfs list -t snapshot -d 1 -H ${LDATA} | tail -1 | awk '{$1=$1}1' OFS=" " | cut -d '@' -f 2 | cut -d ' ' -f 1`
 
  if [ "$lastSEND" = "$lastSNAP" ] ; then
     queue_msg "`date`: Last snapshot $lastSNAP is already marked as replicated!"
     echo_log "Finished replication task on ${DATASET} -> ${REPHOST}"
     rm ${pidFile}
     return 0
  fi

  # Grab a list of snapshots from remote
  _rSnapList="/tmp/.remoteZSnap.$$"
  ${CMDPREFIX} zfs list -r -t snap -o name ${REMOTEDSET}/${hName} 2>/dev/null | sed "s|^${REMOTEDSET}/${hName}||g" >${_rSnapList}

  # Get list of datasets
  build_dset_list "$LDATA" "rep"

  # Get list of remote datasets
  _rDsetList="/tmp/.remoteDSets.$$"
  ${CMDPREFIX} zfs list -H -r -o name,origin ${REMOTEDSET}/${hName} 2>/dev/null | sed "s|^${REMOTEDSET}/${hName}||g" >${_rDsetList}
  sync

  # Remote any remote datasets that dont exist on host for replication
  prune_remote_datasets "${_rDsetList}"

  # Now go through all datasets and do the replication
  for dset in ${DSETS}
  do

    # If we are on the top level dataset, remove the name from what we receive
    rdset=""
    unset rdsetorigin
    unset ldsetorigin

    # If on a sub-dataset, check the origin property for clones
    removedRemote=0
    if [ "$dset" != "$LDATA" ] ; then
      # Strip off the zpool/top level name
      rdset=`echo $dset | sed "s|^${LDATA}||g"`

      # Check if we have a remote dataset origin
      rdsetorigin=`cat ${_rDsetList} | tr -s '\t' ' ' | grep "^${rdset} " | cut -d ' ' -f 2`
      ldsetorigin=`zfs list -H -o origin $dset`

      if [ -n "$rdsetorigin" ] ; then
        # Lets get the clone / origin properties

        rdsetoriginraw=`echo $rdsetorigin | sed "s|^${REMOTEDSET}/${hName}||g"`
        ldsetoriginraw=`echo $ldsetorigin | sed "s|^${LDATA}||g"`

        if [ "$rdsetoriginraw" != "$ldsetoriginraw" ] ; then
           # If the local dataset is now the parent, we can try promoting the remote dataset
           if [ "$ldsetorigin" = "-" ] ; then
              olddsetraw="${LDATA}`echo $rdsetoriginraw | cut -d '@' -f 1`"
              prevldsetorigin=`zfs list -H -d 1 -o origin ${olddsetraw} | sed "s|${LDATA}||g" | cut -d '@' -f 1`
	      if [ "$prevldsetorigin" = "$rdset" ] ; then
                queue_msg "`date`: Promoting ${REMOTEDSET}/${hName}${rdset}"
		${CMDPREFIX} zfs promote ${REMOTEDSET}/${hName}${rdset}
		zStatus=$?
		if [ $zStatus -ne 0 ] ; then
           break
         fi

		# Since we just juggled origins, need to regen the remote dset list
		${CMDPREFIX} zfs list -H -r -o name,origin ${REMOTEDSET}/${hName} 2>/dev/null | sed "s|^${REMOTEDSET}/${hName}||g" >${_rDsetList}
		sync
	      else
		# Doesn't look like a promotion, need to remove remote dset and resend
                removedRemote=1
              fi
	   else
              # Doesn't look like a promotion, need to remove remote dset and resend
              removedRemote=1
           fi
	fi
        if [ "$removedRemote" = "1" ] ; then
          queue_msg "`date`: Removing ${REMOTEDSET}/${hName}${rdset} - Incorrect origin"
          ${CMDPREFIX} zfs destroy -R ${REMOTEDSET}/${hName}${rdset}
        fi
        if [ "$ldsetorigin" = "-" ] ; then
           unset ldsetorigin
        fi
      else
        if [ "$ldsetorigin" = "-" ] ; then
           unset ldsetorigin
        fi
        removedRemote=1
      fi # End of if rdsetorigin exists
    fi

    # Get the last snapshot that was sent on this dataset
    lastSENDPART=`zfs get -d 1 lpreserver-part:${REPHOST} ${dset} | grep LATEST | awk '{$1=$1}1' OFS=" " | tail -1 | cut -d '@' -f 2 | cut -d ' ' -f 1`

    # If we haven't replicated this dataset, but do have a remote dset of the same name, nuke the remote
    # most likely the user has deleted and created a new dataset of the same name
    if [ -z "$lastSENDPART" -a -n "$rdsetorigin" -a "$removedRemote" = "0" ] ; then
      queue_msg "`date`: Removing ${REMOTEDSET}/${hName}${rdset} - re-created locally"
      echo "`date`: Removing ${REMOTEDSET}/${hName}${rdset} - re-created locally"
      ${CMDPREFIX} zfs destroy -R ${REMOTEDSET}/${hName}${rdset}
      removedRemote=1
    fi

    # If we have a last send status, and didn't remove it for other reason
    if [ -n "$lastSENDPART" -a "$removedRemote" = "0" ] ; then
       # This one is already replicated, lets skip it
       if [ "${lastSENDPART}" = "${lastSNAP}" ] ; then
         #echo "SKIPPING $dset - $lastSENDPART - $lastSNAP"
         queue_msg "`date`: Skipping ${dset}@$lastSNAP, is already marked as replicated!"
	 continue;
       fi
    fi
    if [ "$removedRemote" = "1" ] ; then
      if [ -n "$lastSENDPART" ] ; then
        zfs set lpreserver-part:${REPHOST}=' ' ${dset}@$lastSENDPART
      fi
      lastSENDPART=""
    fi

    # Check the remote, if the snapshot already exists, we can skip this one
    grep -q "^${rdset}@${lastSNAP}" ${_rSnapList}
    if [ $? -eq 0 ] ; then
      queue_msg "`date`: Skipping already replicated: ${dset}@${lastSNAP}"
      if [ -n "${lastSEND}" ] ; then
        zfs set lpreserver-part:${REPHOST}=' ' ${dset}@${lastSEND}
      fi
      zfs set lpreserver-part:${REPHOST}=LATEST ${dset}@${lastSNAP}
      continue
    fi

    zfs list -H -d 1 -t snap -o name ${dset} | grep -q "${dset}@${lastSNAP}"
    if [ $? -ne 0 ] ; then
      #echo "`date`: Skipping ${dset}, does not have latest snapshot: ${lastSNAP}"
      queue_msg "`date`: Skipping ${dset}, does not have latest snapshot: ${lastSNAP}"
      continue
    fi

    unset firstSNAP
    unset zFLAGSFIRST
    zFLAGSRCV="-u -v -F ${REMOTEDSET}/${hName}${rdset}"
    # Starting replication, first lets check if we can do an incremental send
    if [ -n "$lastSENDPART" ] ; then
       zFLAGS="-v -I ${lastSENDPART} ${dset}@${lastSNAP}"
       # Check if we have any old snapshots to prune
       prune_old_remote_snaps "$dset" "${REMOTEDSET}/${hName}${rdset}"
    else
       # If the local dataset is the parent, we can create
       if [ -z "$ldsetorigin" -o "$ldsetorigin" = "-" ] ; then
         firstSNAP="`zfs list -H -d 1 -t snapshot -o name ${dset} | head -n 1 | cut -d '@' -f 2`"
         zFLAGS="-v -I ${firstSNAP} ${dset}@${lastSNAP}"
         zFLAGSFIRST="-v ${dset}@${firstSNAP}"
         ${CMDPREFIX} zfs create -o mountpoint=none -o compression=lz4 ${REMOTEDSET}/${hName}${rdset} >${CMDLOG} 2>${CMDLOG}
         if [ $? -ne 0 ] ; then
           # Try again without compression
           ${CMDPREFIX} zfs create -o mountpoint=none ${REMOTEDSET}/${hName}${rdset} >${CMDLOG} 2>${CMDLOG}
           if [ $? -ne 0 ] ; then
             echo "Failed creating remote dataset!"
             cat ${CMDLOG}
             zStatus=1
             break
           fi
         fi
       else
         # If the local dataset is cloned, we can clone the remote dataset as well (Was previously replicated)
         roriginsnap=`echo $ldsetorigin | sed "s|^${LDATA}||g"`
         zFLAGS="-v -I ${ldsetorigin} ${dset}@${lastSNAP}"
       fi
    fi

    # If we are doing the initial replication, start with first snap
    if [ -n "$firstSNAP" ] ; then
      do_zfs_send_now "$zFLAGSFIRST" "$zFLAGSRCV" "$firstSNAP"
      zStatus=$?
      if [ $zStatus -ne 0 ] ; then break ; fi

      # If the firstSNAP is the same as the lastSNAP then we can skip incremental
      if [ "$firstSNAP" = "$lastSNAP" ] ; then
        # Now that replication of this dataset is done, save properties on remote
        save_remote_props "${dset}"
        zStatus=$?
        if [ $zStatus -ne 0 ] ; then
           break
        fi
        continue
      fi
    fi

    # Do the incremental send/recv now
    do_zfs_send_now "$zFLAGS" "$zFLAGSRCV" "$lastSNAP"
    zStatus=$?
    if [ $zStatus -ne 0 ] ; then break ; fi

    # Now that replication of this dataset is done, save properties on remote
    save_remote_props "${dset}"
    zStatus=$?
    if [ $zStatus -ne 0 ] ; then break ; fi

  done

  if [ -e "${_rSnapList}" ] ; then rm ${_rSnapList}; fi
  if [ -e "${_rDsetList}" ] ; then rm ${_rDsetList}; fi

  cleanup_iscsi

  if [ $zStatus -eq 0 ] ; then
     # SUCCESS!
     # Lets mark our new latest snapshot as the last full replication
     if [ -n "$lastSEND" ] ; then
       zfs set lpreserver:${REPHOST}=' ' ${LDATA}@$lastSEND
     fi
     zfs set lpreserver:${REPHOST}=LATEST ${LDATA}@$lastSNAP
     echo_log "Finished replication task on ${DATASET} -> ${REPHOST}"
     if [ -z "$ISCSI" ] ; then
        save_rep_props
     fi
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
     echo_log "FAILED replication task on ${DATASET} -> ${REPHOST}: LOGFILE: $FLOG"
     queue_msg "FAILED replication task on ${DATASET} -> ${REPHOST}: LOGFILE: $FLOG"
     queue_msg "`cat ${FLOG}`"
  fi

  rm ${pidFile}
  return $zStatus
}

do_zfs_send_now() {

    # Do the send/recv now
    zSEND="zfs send ${1}"
    zRCV="${CMDPREFIX} zfs receive $2"
    queue_msg "Using ZFS send command:\n$zSEND | $zRCV\n\n"

    # Start up our process
    $zSEND 2>${REPLOGSEND} | $zRCV >${REPLOGRECV} 2>${REPLOGRECV}
    local rtnCode=$?

    queue_msg "ZFS SEND LOG:\n--------------\n" "${REPLOGSEND}"
    queue_msg "ZFS RCV LOG:\n--------------\n" "${REPLOGRECV}"

    if [ $rtnCode -ne 0 ] ; then
       return $rtnCode
    fi

    if [ -n "$lastSENDPART" ] ; then
      zfs set lpreserver-part:${REPHOST}=' ' ${dset}@$lastSENDPART
    fi
    zfs set lpreserver-part:${REPHOST}=LATEST ${dset}@${3}

    return 0
}

save_rep_props() {

  # If we are not doing a recursive backup / complete dataset we can skip this
  if [ "$RECURMODE" != "ON" ] ; then
     return 0
  fi
  if [ "`basename $DATASET`" != "$DATASET" ] ; then
     return 0
  fi
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

      if [ -z "$lastSEND" ] ; then
         lastSEND="NONE"
      fi
      if [ -z "$lastSNAP" ] ; then
         lastSNAP="NONE"
      fi
      echo "$i -> $REPHOST - $lastSNAP - $lastSEND"
    done
  done
}

expand_iscsi_zpool() {
  LDATA="$1"
  if [ -z "$1" -o -z "$2" ] ; then
     exit_err "Usage: lpreserver replicate expand <zpool> <target host>"
  fi

  repLine=`cat ${REPCONF} | grep "^${LDATA}:.*:${2}:"`
  if [ -z "$repLine" ] ; then exit_err "No such replication task: ${LDATA}";fi

  # We have a replication task for this set, get some vars
  hName=`hostname`
  REPHOST=`echo $repLine | cut -d ':' -f 3`
  REPUSER=`echo $repLine | cut -d ':' -f 4`
  REPPORT=`echo $repLine | cut -d ':' -f 5`
  REPRDATA=`echo $repLine | cut -d ':' -f 6`

  if [ "$REPRDATA" != "ISCSI" ] ; then
     exit_err "Not a iSCSI target"
  fi

  load_iscsi_rep_data
  connect_iscsi "connectonly"
  if [ $? -ne 0 ] ; then
     exit_err "Failed connecting to iscsi device!"
  fi

  # Get the original size
  origSize=`gpart show ${diskName} | grep ' 1 ' | awk '{print $5}' | cut -d '(' -f 2 | cut -d ')' -f 1`

  # Do the resize now
  rc_halt "gpart resize -i 1 ${diskName}"
  rc_halt "geli resize -s ${origSize} ${diskPart}"

  # Now connect the zpool
  connect_geli_zpool
  if [ $? -ne 0 ] ; then
     exit_err "Failed to import zpool!"
  fi

  # Set the zpool to expand
  rc_halt "zpool set autoexpand=on ${REPPOOL}"

  # Now we can finish up
  cleanup_iscsi

  echo "Backup ZFS pool was expanded!"

  exit 0
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

  if [ "$REPRDATA" = "ISCSI" ] ; then
     load_iscsi_rep_data
     connect_iscsi "init"
     if [ $? -ne 0 ] ; then
	cat ${CMDLOG}
        cleanup_iscsi
	exit_err "Failed importing the iSCSI volume..."
     fi

     zpool destroy ${REPPOOL}
     if [ $? -ne 0 ] ; then echo "Failed destroying zpool: ${REPPOOL}"; fi

     # Make sure the new zpool uses 4k sector size
     sysctl vfs.zfs.min_auto_ashift=12 >/dev/null 2>/dev/null
     zpool create $ZPOOLFLAGS -m none ${REPPOOL} ${geliPart}
     if [ $? -ne 0 ] ; then echo "Failed creating pool: $geliPart" ; fi

     cleanup_iscsi
  else
    # First check if we even have a dataset on the remote
    ssh -p ${REPPORT} ${REPUSER}@${REPHOST} zfs list ${REPRDATA}/${hName} 2>/dev/null >/dev/null
    if [ $? -eq 0 ] ; then
       # Lets cleanup the remote side
       echo "Removing remote dataset: ${REPRDATA}/${hName}"
       ssh -p ${REPPORT} ${REPUSER}@${REPHOST} zfs destroy -R ${REPRDATA}/${hName}
       if [ $? -ne 0 ] ; then
          echo "Warning: Could not delete remote dataset ${REPRDATA}/${hName}"
       fi
    fi
  fi

  # Now lets mark none of our datasets as replicated
  while :
  do
    lastSEND=`zfs get -d 1 lpreserver:${REPHOST} ${LDATA} | grep LATEST | awk '{$1=$1}1' OFS=" " | tail -1 | cut -d '@' -f 2 | cut -d ' ' -f 1`
    if [ -n "$lastSEND" ] ; then
       zfs set lpreserver:${REPHOST}=' ' ${LDATA}@$lastSEND
       continue
    fi
    break
  done

  build_dset_list "$LDATA" "rep"
  for dset in ${DSETS}
  do
    while :
    do
      lastSENDPART=`zfs get -d 1 lpreserver-part:${REPHOST} ${dset} | grep LATEST | awk '{$1=$1}1' OFS=" " | tail -1 | cut -d '@' -f 2 | cut -d ' ' -f 1`
      if [ -n "$lastSENDPART" ] ; then
        zfs set lpreserver-part:${REPHOST}=' ' ${dset}@$lastSENDPART
        continue
      fi
      break
    done
  done

  echo "Ready to do full replication for: $LDATA"
}

## Function to remove the oldest life-preserver snapshot on the target
## zpool, used by zfsmon.sh when the disk space gets low
do_pool_cleanup()
{
  # Is this zpool managed by life-preserver?
  grep -q "${PROGDIR}/backend/runsnap.sh ${1} " /etc/crontab
  if [ $? -ne 0 ] ; then
     return
  fi

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
     if [ "$cur" != "auto" ] ; then
        continue
     fi

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

import_iscsi_zpool() {

  LDATA="$1"
  if [ -z "$1" -o -z "$2" ] ; then
     exit_err "Usage: lpreserver replicate import <zpool> <target host>"
  fi

  repLine=`cat ${REPCONF} | grep "^${LDATA}:.*:${2}:"`
  if [ -z "$repLine" ] ; then
     exit_err "No such replication task: ${LDATA}"
  fi
 
  # We have a replication task for this set, get some vars
  hName=`hostname`
  REPHOST=`echo $repLine | cut -d ':' -f 3`
  REPUSER=`echo $repLine | cut -d ':' -f 4`
  REPPORT=`echo $repLine | cut -d ':' -f 5`
  REPRDATA=`echo $repLine | cut -d ':' -f 6`

  if [ "$REPRDATA" != "ISCSI" ] ; then
    exit_err "This replication is not an iSCSI volume"
  fi

  load_iscsi_rep_data
  connect_iscsi
  if [ $? -ne 0 ] ; then
    cat ${CMDLOG}
    cleanup_iscsi
    exit_err "Failed importing the iSCSI volume..."
  fi

  echo "iSCSI zpool has been imported to ${REPPOOL}"
  exit 0
}

export_iscsi_zpool() {

  LDATA="$1"
  if [ -z "$1" -o -z "$2" ] ; then
     exit_err "Usage: lpreserver replicate export <zpool> <target host>"
  fi

  repLine=`cat ${REPCONF} | grep "^${LDATA}:.*:${2}:"`
  if [ -z "$repLine" ] ; then
     exit_err "No such replication task: ${LDATA}"
  fi
 
  # We have a replication task for this set, get some vars
  hName=`hostname`
  REPHOST=`echo $repLine | cut -d ':' -f 3`
  REPUSER=`echo $repLine | cut -d ':' -f 4`
  REPPORT=`echo $repLine | cut -d ':' -f 5`
  REPRDATA=`echo $repLine | cut -d ':' -f 6`

  if [ "$REPRDATA" != "ISCSI" ] ; then
    exit_err "This replication is not an iSCSI volume"
  fi

  load_iscsi_rep_data
  
  connect_iscsi
  export startISCSI="1"
  cleanup_iscsi

  echo "The iSCSI zpool ($REPPOOL) has been exported"
  exit 0
}

save_iscsi_zpool_data() {
  LDATA="$1"
  if [ -z "$1" -o -z "$2" ] ; then
     exit_err "Usage: lpreserver replicate saveiscsi <zpool> <target host> [password file]"
  fi
  PASSFILE="$3"

  repLine=`cat ${REPCONF} | grep "^${LDATA}:.*:${2}:"`
  if [ -z "$repLine" ] ; then
     exit_err "No such replication task: ${LDATA}"
  fi

  # We have a replication task for this set, get some vars
  hName=`hostname`
  REPHOST=`echo $repLine | cut -d ':' -f 3`
  REPUSER=`echo $repLine | cut -d ':' -f 4`
  REPPORT=`echo $repLine | cut -d ':' -f 5`
  REPRDATA=`echo $repLine | cut -d ':' -f 6`

  if [ "$REPRDATA" != "ISCSI" ] ; then
    exit_err "This replication is not an iSCSI volume"
  fi

  load_iscsi_rep_data

  # Make sure we have values to save
  if [ -z "$REPHOST" -o -z "$REPUSER" -o -z "$REPPORT" -o -z "$REPRDATA" -o -z "$REPTARGET" -o -z "$REPPOOL" -o -z "$REPINAME" -o -z "$REPPASS" ] ; then
    exit_err "Failed sanity-check of replication meta-data!"
  fi

  SANELDATA="`echo $LDATA | sed 's|/|-|g'`"
  LPFILE="lp-`hostname`-${SANELDATA}-${REPHOST}.lpiscsi"

  truncate -s 5M ${LPFILE}
  MD=`mdconfig -t vnode -f ${LPFILE}`
  if [ -n "$PASSFILE" ] ; then
    echo "Creating GELI provider..."
    cat ${PASSFILE} | geli init -J - ${MD} >/dev/null 2>/dev/null
    if [ $? -ne 0 ] ; then
       mdconfig -d -u $MD
       rm ${LPFILE}
       exit_err "Failed GELI init"
    fi
    cat ${PASSFILE} | geli attach -j - ${MD} >/dev/null 2>/dev/null
    if [ $? -ne 0 ] ; then
       mdconfig -d -u $MD
       rm ${LPFILE}
       exit_err "Failed GELI attach"
    fi
    rm ${PASSFILE}
  else
    echo "Please enter the password for this backup iscsi file:"
    echo -e ">\c"
    stty -echo
    read PASSWORD
    stty echo
    printf "\n"
    echo "Repeat Password"
    echo -e ">\c"
    stty -echo
    read PASSWORD2
    stty echo
    printf "\n"
    if [ -z "$PASSWORD" -o "$PASSWORD" != "$PASSWORD2" ] ; then
       mdconfig -d -u $MD
       rm ${LPFILE}
       exit_err "Password mismatch!"
    fi
    echo "Creating GELI provider..."
    echo "$PASSWORD" | geli init -J - ${MD} >/dev/null 2>/dev/null
    if [ $? -ne 0 ] ; then
       mdconfig -d -u $MD
       rm ${LPFILE}
       exit_err "Failed GELI init"
    fi
    echo "$PASSWORD" | geli attach -j - ${MD} >/dev/null 2>/dev/null
    if [ $? -ne 0 ] ; then
       mdconfig -d -u $MD
       rm ${LPFILE}
       exit_err "Failed GELI attach"
    fi
  fi

  # Setup FS
  echo "Creating file-system..."
  newfs /dev/${MD}.eli >/dev/null 2>/dev/null
  if [ $? -ne 0 ] ; then
    geli stop /dev/${MD}.eli
    mdconfig -d -u $MD
    rm ${LPFILE}
    exit_err "Failed newfs"
  fi

  MNTDIR=`mktemp -d /tmp/XXXXXXXXXXXXXXXXXXX`
  mount /dev/${MD}.eli ${MNTDIR}
  if [ $? -ne 0 ] ; then
    geli stop /dev/${MD}.eli
    mdconfig -d -u $MD
    rm ${LPFILE}
    exit_err "Failed mounting"
  fi

  # Save the meta-data
  echo "$REPHOST" > ${MNTDIR}/REPHOST
  echo "$REPUSER" > ${MNTDIR}/REPUSER
  echo "$REPPORT" > ${MNTDIR}/REPPORT
  echo "$REPRDATA" > ${MNTDIR}/REPRDATA
  echo "$REPTARGET" > ${MNTDIR}/REPTARGET
  echo "$REPPOOL" > ${MNTDIR}/REPPOOL
  echo "$REPINAME" > ${MNTDIR}/REPINAME
  echo "$REPPASS" > ${MNTDIR}/REPPASS
  cp ${REPGELIKEY} ${MNTDIR}/GELIKEY

  # Now lets read the data back
  if [ "$REPHOST" != "`cat ${MNTDIR}/REPHOST`" ] ; then
    geli stop /dev/${MD}.eli
    mdconfig -d -u $MD
    rm ${LPFILE}
    exit_err "Failed sanity-check of copied replication meta-data!"
  fi
  if [ "$REPUSER" != "`cat ${MNTDIR}/REPUSER`" ] ; then
    geli stop /dev/${MD}.eli
    mdconfig -d -u $MD
    rm ${LPFILE}
    exit_err "Failed sanity-check of copied replication meta-data!"
  fi
  if [ "$REPPORT" != "`cat ${MNTDIR}/REPPORT`" ] ; then
    geli stop /dev/${MD}.eli
    mdconfig -d -u $MD
    rm ${LPFILE}
    exit_err "Failed sanity-check of copied replication meta-data!"
  fi
  if [ "$REPRDATA" != "`cat ${MNTDIR}/REPRDATA`" ] ; then
    geli stop /dev/${MD}.eli
    mdconfig -d -u $MD
    rm ${LPFILE}
    exit_err "Failed sanity-check of copied replication meta-data!"
  fi
  if [ "$REPTARGET" != "`cat ${MNTDIR}/REPTARGET`" ] ; then
    geli stop /dev/${MD}.eli
    mdconfig -d -u $MD
    rm ${LPFILE}
    exit_err "Failed sanity-check of copied replication meta-data!"
  fi
  if [ "$REPPOOL" != "`cat ${MNTDIR}/REPPOOL`" ] ; then
    geli stop /dev/${MD}.eli
    mdconfig -d -u $MD
    rm ${LPFILE}
    exit_err "Failed sanity-check of copied replication meta-data!"
  fi
  if [ "$REPINAME" != "`cat ${MNTDIR}/REPINAME`" ] ; then
    geli stop /dev/${MD}.eli
    mdconfig -d -u $MD
    rm ${LPFILE}
    exit_err "Failed sanity-check of copied replication meta-data!"
  fi
  if [ "$REPPASS" != "`cat ${MNTDIR}/REPPASS`" ] ; then
    geli stop /dev/${MD}.eli
    mdconfig -d -u $MD
    rm ${LPFILE}
    exit_err "Failed sanity-check of copied replication meta-data!"
  fi

  # Make sure GELI key is good
  diff -q ${REPGELIKEY} ${MNTDIR}/GELIKEY >/dev/null 2>/dev/null
  if [ $? -ne 0 ] ; then
    geli stop /dev/${MD}.eli
    mdconfig -d -u $MD
    rm ${LPFILE}
    exit_err "Failed sanity-check of copied replication GELI key!"
  fi

  umount -f ${MNTDIR}
  rmdir ${MNTDIR}
  geli stop /dev/${MD}.eli
  mdconfig -d -u ${MD}

  echo "iSCSI config and GELI key saved to: $LPFILE"
  echo ""
  echo "!! -- PLEASE KEEP THIS IN A SAFE LOCATION -- !!"
  echo ""
  echo "If you lose the password of this file you will be unable"
  echo "to restore your data!"

  exit 0
}
