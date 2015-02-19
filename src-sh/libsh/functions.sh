#!/bin/sh
# Functions we can source for pc-bsd scripts
# Author: Kris Moore
# Copyright: 2012
# License: BSD
##############################################################

PCBSD_ETCCONF="/usr/local/etc/pcbsd.conf"

get_mirror() {

  # Check if we already looked up a mirror we can keep using
  if [ -n "$CACHED_PCBSD_MIRROR" ] ; then
     VAL="$CACHED_PCBSD_MIRROR"
     export VAL
     return
  fi

  # Set the mirror URL
  VAL="`cat ${PCBSD_ETCCONF} 2>/dev/null | grep 'PCBSD_MIRROR: ' | sed 's|PCBSD_MIRROR: ||g'`"
  if [ -n "$VAL" ] ; then
     echo "Using mirror: $VAL"
     CACHED_PCBSD_MIRROR="$VAL"
     export VAL CACHED_PCBSD_MIRROR
     return
  fi

  echo "Getting regional mirror..."
  . /etc/profile

  # No URL? Lets get one from the master server
  local mFile="${HOME}/.mirrorUrl.$$"
  touch $mFile
  fetch -o $mFile http://getmirror.pcbsd.org >/dev/null 2>/dev/null
  VAL="`cat $mFile | grep 'URL: ' | sed 's|URL: ||g'`"
  rm $mFile
  if [ -n "$VAL" ] ; then
     echo "Using mirror: $VAL"
     CACHED_PCBSD_MIRROR="$VAL"
     export VAL CACHED_PCBSD_MIRROR
     return
  fi

  # Still no mirror? Lets try the PC-BSD FTP server...
  VAL="ftp://ftp.pcbsd.org/pub/mirror"
  CACHED_PCBSD_MIRROR="$VAL"
  export VAL CACHED_PCBSD_MIRROR
  echo "Using mirror: $VAL"
  return 
}

# Function which returns the installed list of PC-BSD mirrors for use
# with the fetch command
# Will return just a single mirror, if the user has manually specified one
# in /usr/local/etc/pcbsd.conf
get_mirror_loc()
{
  if [ -z $1 ] ; then
     exit_err "Need to supply file to grab from mirrors..."
  fi
  if [ -z $2 ] ; then
     exit_err "Need to supply which mirror to fetch from..."
  fi

  case $2 in
    pkg) mirrorTag="PKG_MIRROR" 
         mirrorFile="/usr/local/share/pcbsd/conf/pkg-mirror"
         ;;
    pbi) mirrorTag="PBI_MIRROR" 
         mirrorFile="/usr/local/share/pcbsd/conf/pbi-mirror"
         ;;
    iso) mirrorTag="ISO_MIRROR" 
         mirrorFile="/usr/local/share/pcbsd/conf/iso-mirror"
         ;;
  update) mirrorTag="UPDATE_MIRROR" 
         mirrorFile="/usr/local/share/pcbsd/conf/update-mirror"
         ;;
    *) exit_err "Bad mirror type!" ;;
  esac

  # Set the mirror URL
  local VAL=`cat ${PCBSD_ETCCONF} 2>/dev/null | grep "^${mirrorTag}:" | sed "s|^${mirrorTag}: ||g"`
  if [ -n "$VAL" ] ; then
     echo "${VAL}${1}"
     return
  fi

  if [ ! -e "${mirrorFile}" ] ; then
     exit_err "Missing mirror list: ${mirrorFile}"
  fi

  # Build the mirror list
  while read line
  do
    VAL="${line}${1}"
    break
  done < ${mirrorFile}
  echo ${VAL}
}

# Function to download a file from the pcbsd mirrors
# Arg1 = Remote File URL
# Arg2 = Where to save file
get_file_from_mirrors()
{
   _rf="${1}"
   _lf="${2}"
   _mtype="${3}"

   case $_mtype in
      iso|pbi|pkg|update) ;;
      *) exit_err "Fixme! Missing mirror type in get_file_from_mirrors" ;;
   esac

   # Get any proxy information
   . /etc/profile

   # Get mirror list
   local mirrorLoc="$(get_mirror_loc ${_rf} ${_mtype})"
   mirrorLoc="`echo $mirrorLoc | awk '{print $1}'`"

   # Running from a non GUI?
   if [ "$GUI_FETCH_PARSING" != "YES" -a "$PBI_FETCH_PARSING" != "YES" -a -z "$PCFETCHGUI" ] ; then
      fetch -o "${_lf}" ${mirrorLoc}
      return $?
   fi

   echo "FETCH: ${_rf}"

   # Doing a front-end download, parse the output of fetch
   _eFile="/tmp/.fetch-exit.$$"
   fetch -s ${mirrorLoc} > /tmp/.fetch-size.$$ 2>/dev/null
   _fSize=`cat /tmp/.fetch-size.$$ 2>/dev/null`
   _fSize="`expr ${_fSize} / 1024 2>/dev/null`"
   rm "/tmp/.fetch-size.$$" 2>/dev/null
   _time=1
   if [ -z "$_fSize" ] ; then _fSize=0; fi

   ( fetch -o ${_lf} ${mirrorLoc} >/dev/null 2>/dev/null ; echo "$?" > ${_eFile} ) &
   FETCH_PID=$!
   while : 
   do
      if [ -e "${_lf}" ] ; then
         sync
         _dSize=`du -k ${_lf} | tr -d '\t' | cut -d '/' -f 1`
         if [ $(is_num "$_dSize") ] ; then
            if [ ${_fSize} -lt ${_dSize} ] ; then _dSize="$_fSize" ; fi
	    _kbs=`expr ${_dSize} \/ $_time`
	    echo "SIZE: ${_fSize} DOWNLOADED: ${_dSize} SPEED: ${_kbs} KB/s"
  	 fi
      fi

      # Make sure download isn't finished
      jobs -l >/tmp/.jobProcess.$$
      cat /tmp/.jobProcess.$$ | awk '{print $3}' | grep -q ${FETCH_PID}
      if [ "$?" != "0" ] ; then rm /tmp/.jobProcess.$$ ; break ; fi
      sleep 1
      _time=`expr $_time + 1`
   done

   _err="`cat ${_eFile} 2>/dev/null`"
   if [ -z "$_err" ] ; then _err="0"; fi
   rm ${_eFile} 2>/dev/null
   if [ "$_err" = "0" ]; then echo "FETCHDONE" ; fi
   unset FETCH_PID
   return $_err

}

# Function to download a file from remote using fetch
# Arg1 = Remote File URL
# Arg2 = Where to save file
# Arg3 = Number of attempts to make before failing
get_file() {

	_rf="${1}"
	_lf="${2}"
        _ftries=${3}
	if [ -z "$_ftries" ] ; then _ftries=3; fi

        # Get any proxy information
        . /etc/profile

	if [ -e "${_lf}" ] ; then 
		echo "Resuming download of: ${_lf}"
	fi

	if [ "$GUI_FETCH_PARSING" != "YES" -a -z "$PCFETCHGUI" ] ; then
		fetch -r -o "${_lf}" "${_rf}"
		_err=$?
	else
		echo "FETCH: ${_rf}"

		# Doing a front-end download, parse the output of fetch
		_eFile="/tmp/.fetch-exit.$$"
		fetch -s "${_rf}" > /tmp/.fetch-size.$$ 2>/dev/null
		_fSize=`cat /tmp/.fetch-size.$$ 2>/dev/null`
		_fSize="`expr ${_fSize} / 1024 2>/dev/null`"
		rm "/tmp/.fetch-size.$$" 2>/dev/null
		_time=1
   		if [ -z "$_fSize" ] ; then _fSize=0; fi

		( fetch -r -o "${_lf}" "${_rf}" >/dev/null 2>/dev/null ; echo "$?" > ${_eFile} ) &
		FETCH_PID=`ps -auwwwx | grep -v grep | grep "fetch -r -o ${_lf}" | awk '{print $2}'`
		while : 
		do
			if [ -e "${_lf}" ] ; then
				_dSize=`du -k ${_lf} | tr -d '\t' | cut -d '/' -f 1`
				if [ $(is_num "$_dSize") ] ; then
					if [ ${_fSize} -lt ${_dSize} ] ; then _dSize="$_fSize" ; fi
					_kbs=`expr ${_dSize} \/ $_time`
					echo "SIZE: ${_fSize} DOWNLOADED: ${_dSize} SPEED: ${_kbs} KB/s"
				fi
			fi

			# Make sure download isn't finished
			ps -p $FETCH_PID >/dev/null 2>/dev/null
			if [ "$?" != "0" ] ; then break ; fi
			sleep 2
			_time=`expr $_time + 2`
		done

		_err="`cat ${_eFile} 2>/dev/null`"
		if [ -z "$_err" ] ; then _err="0"; fi
                rm ${_eFile} 2>/dev/null
		if [ "$_err" = "0" ]; then echo "FETCHDONE" ; fi
		unset FETCH_PID
	fi

	echo ""
	if [ $_err -ne 0 -a $_ftries -gt 0 ] ; then
		sleep 30
		_ftries=`expr $_ftries - 1`

		# Remove the local file if we failed
		if [ -e "${_lf}" ]; then rm "${_lf}"; fi

		get_file "${_rf}" "${_lf}" $_ftries	
		_err=$?
	fi
	return $_err
}

# Check if a value is a number
is_num()
{
        expr $1 + 1 2>/dev/null
        return $?
}

# Exit with a error message
exit_err() {
	if [ -n "${LOGFILE}" ] ; then
           echo "ERROR: $*" >> ${LOGFILE}
	fi
  	echo >&2 "ERROR: $*"
        exit 1
}


### Print an error on STDERR and bail out
printerror() {
  exit_err $*
}


# Check if the target directory is on ZFS
# Arg1 = The dir to check
# Arg2 = If set to 1, don't dig down to lower level directory
isDirZFS() {
  local _chkDir="$1"
  while :
  do
     # Is this dir a ZFS mount
     mount | grep -w "on $_chkDir " | grep -qw "(zfs," && return 0

     # If this directory is mounted, but NOT ZFS
     if [ "$2" != "1" ] ; then
       mount | grep -qw "on $_chkDir " && return 1
     fi
      
     # Quit if not walking down
     if [ "$2" = "1" ] ; then return 1 ; fi
  
     if [ "$_chkDir" = "/" ] ; then break ; fi
     _chkDir=`dirname $_chkDir`
  done
  
  return 1
}

# Gets the mount-point of a particular zpool / dataset
# Arg1 = zpool to check
getZFSMount() {
  local zpool="$1"
  local mnt=`mount | grep "^${zpool} on" | grep "(zfs," | awk '{print $3}'`
  if [ -n "$mnt" ] ; then
     echo "$mnt"
     return 0
  fi
  return 1
}

# Get the ZFS dataset of a particular directory
getZFSDataset() {
  local _chkDir="$1"
  while :
  do
    local zData=`mount | grep " on ${_chkDir} " | grep "(zfs," | awk '{print $1}'`
    if [ -n "$zData" ] ; then
       echo "$zData"
       return 0
    fi
    if [ "$2" != "rec" ] ; then return 1 ; fi
    if [ "$_chkDir" = "/" ] ; then return 1 ; fi
    _chkDir=`dirname $_chkDir`
  done
  return 1
}

# Get the ZFS tank name for a directory
# Arg1 = Directory to check
getZFSTank() {
  local _chkDir="$1"

  _chkdir=${_chkDir%/}
  while :
  do
     zpath=`zfs list | awk -v path="${_chkDir}" '$5 == path { print $1 }'`
     if [ -n "${zpath}" ] ; then
        echo $zpath | cut -f1 -d '/'
        return 0
     fi

     if [ "$_chkDir" = "/" ] ; then return 1 ; fi
     _chkDir=`dirname $_chkDir`
  done

  return 1
}

# Get the mountpoint for a ZFS name
# Arg1 = name
getZFSMountpoint() {
   local _chkName="${1}"
   if [ -z "${_chkName}" ]; then return 1 ; fi

   zfs list "${_chkName}" | tail -1 | awk '{ print $5 }'
}

# Get the ZFS relative path for a path
# Arg1 = Path
getZFSRelativePath() {
   local _chkDir="${1}"
   local _tank=`getZFSTank "$_chkDir"`
   local _mp=`getZFSMountpoint "${_tank}"`

   if [ -z "${_tank}" ] ; then return 1 ; fi

   local _name="${_chkDir#${_mp}}"

   # Make sure we have a '/' at the start of dataset
   if [ "`echo ${_name} | cut -c 1`" != "/" ] ; then _name="/${_name}"; fi

   echo "${_name}"
   return 0
}

# Check if an address is IPv6
isV6() {
  echo ${1} | grep -q ":"
  return $?
}
    
# Is a mount point, or any of its parent directories, a symlink?
is_symlinked_mountpoint()
{
        local _dir
        _dir=$1
        [ -L "$_dir" ] && return 0
        [ "$_dir" = "/" ] && return 1
        is_symlinked_mountpoint `dirname $_dir`
        return $?
}

# Function to ask the user to press Return to continue
rtn()
{
  echo -e "Press ENTER to continue\c";
  read garbage
};

# Function to check if an IP address passes a basic sanity test
check_ip()
{
  ip="$1"
  
  # If this is a V6 address, skip validation for now
  isV6 "${ip}"
  if [ $? -eq 0 ] ; then return ; fi

  # Check if we can cut this IP into the right segments 
  SEG="`echo $ip | cut -d '.' -f 1 2>/dev/null`"
  echo $SEG | grep -E "^[0-9]+$" >/dev/null 2>/dev/null
  if [ "$?" != "0" ]
  then
     return 1
  fi
  if [ $SEG -gt 255 -o $SEG -lt 0 ]
  then
     return 1
  fi
  
  # Second segment
  SEG="`echo $ip | cut -d '.' -f 2 2>/dev/null`"
  echo $SEG | grep -E "^[0-9]+$" >/dev/null 2>/dev/null
  if [ "$?" != "0" ]
  then
     return 1
  fi
  if [ $SEG -gt 255 -o $SEG -lt 0 ]
  then
     return 1
  fi

  # Third segment
  SEG="`echo $ip | cut -d '.' -f 3 2>/dev/null`"
  echo $SEG | grep -E "^[0-9]+$" >/dev/null 2>/dev/null
  if [ "$?" != "0" ]
  then
     return 1
  fi
  if [ $SEG -gt 255 -o $SEG -lt 0 ]
  then
     return 1
  fi
  
  # Fourth segment
  SEG="`echo $ip | cut -d '.' -f 4 2>/dev/null`"
  echo $SEG | grep -E "^[0-9]+$" >/dev/null 2>/dev/null
  if [ "$?" != "0" ]
  then
     return 1
  fi
  if [ $SEG -gt 255 -o $SEG -lt 0 ]
  then
     return 1
  fi

  return 0
};

# Run the first boot wizard
# Should be called from a .xinitrc script, after fluxbox is already running
run_firstboot()
{
  # Is the trigger file set?
  if [ ! -e "/var/.pcbsd-firstgui" ] ; then return; fi

  # Set all our path variables
  PATH="/sbin:/bin:/usr/sbin:/usr/bin:/root/bin:/usr/local/bin:/usr/local/sbin"
  HOME="/root"
  export PATH HOME

  # Unset the PROGDIR variable
  PROGDIR=""
  export PROGDIR

  if [ -e "/root/.xprofile" ] ; then . /root/.xprofile ; fi

  # Figure out which intro video to play
  res=`xdpyinfo | grep dimensions: | awk "{print $2}"`
  h=`echo $res | cut -d "x" -f 1`
  w=`echo $res | cut -d "x" -f 2`
  h=`expr 100 \* $h`
  ratio=`expr $h \/ $w | cut -c 1-2`
  case $ratio in
    13) mov="PCBSD9_4-3_UXGA.flv";;
    16) mov="PCBSD9_16-10_WUXGA.flv";;
    17) mov="PCBSD9_16-9_1080p.flv";;
     *) mov="PCBSD9_4-3_UXGA.flv";;
  esac

  # Play the video now
  # NO Movie for 10, if we end up with one, replace this
  #mplayer -fs -nomouseinput -zoom /usr/local/share/pcbsd/movies/$mov

  # Setting a language
  if [ -e "/etc/pcbsd-lang" ] ; then
    LANG=`cat /etc/pcbsd-lang`
    export LANG
  fi

  # Start first-boot wizard
  /usr/local/bin/pc-firstboot >/var/log/pc-firstbootwiz 2>/var/log/pc-firstbootwiz
  if [ $? -eq 0 ] ; then
    rm /var/.pcbsd-firstgui
  fi
}

# Run-command, don't halt if command exits with non-0
rc_nohalt()
{
  CMD="$1"

  if [ -z "${CMD}" ] ; then
    exit_err "Error: missing argument in rc_nohalt()"
  fi

  ${CMD}
}

# Run-command, halt if command exits with non-0
rc_halt()
{
  CMD="$@"

  if [ -z "${CMD}" ] ; then
    exit_err "Error: missing argument in rc_halt()"
  fi

  ${CMD}
  STATUS=$?
  if [ ${STATUS} -ne 0 ] ; then
    exit_err "Error ${STATUS}: ${CMD}"
  fi
}

# Run-command silently, only display / halt if command exits with non-0
rc_halt_s()
{
  CMD="$@"

  if [ -z "${CMD}" ] ; then
    exit_err "Error: missing argument in rc_halt()"
  fi

  TMPRCLOG=`mktemp /tmp/.rc_halt.XXXXXX`
  ${CMD} >${TMPRCLOG} 2>${TMPRCLOG}
  STATUS=$?
  if [ ${STATUS} -ne 0 ] ; then
    cat ${TMPRCLOG}
    rm ${TMPRCLOG}
    exit_err "Error ${STATUS}: ${CMD}"
  fi
  rm ${TMPRCLOG}
}

create_auto_beadm()
{
  if [ -n "$NOBEADM" ] ; then return; fi

  echo "Creating new boot-environment..."
  beadm create beforeUpdate-`date "+%Y-%m-%d_%H-%M-%S"`
  if [ $? -ne 0 ] ; then
     echo "WARNING: Unable to create a new boot-enviroment!"
     sleep 10
     return
  fi

}

do_prune_be()
{
  # Check for number of BE's to keep
  MAXBE="3"
  VAL="`cat ${PCBSD_ETCCONF} 2>/dev/null | grep 'MAXBE: ' | sed 's|MAXBE: ||g'`"
  if [ -n "$VAL" ] ; then
     if [ $(is_num "$VAL") ] ; then
       MAXBE="$VAL"
     fi
  fi

  # Check if we need to prune any BEs
  echo "Pruning old boot-environments..."
  bList="`mktemp /tmp/.belist.XXXXXX`"
  beadm list > $bList 2>$bList
  snapList=`cat $bList | grep -e "^beforeUpdate" -e "default" -e "-up-" | awk '{print $1}'`
  snapCount=`cat $bList | grep -e "^beforeUpdate" -e "default" -e "-up-" | awk '{print $1}' | wc -l | awk '{print $1}'`

  if [ -z "$snapCount" ] ; then return ; fi

  # Check if this is forced removal of an old BE
  if [ "$1" = "force" ] ; then
     # If we only have 3 BE's, return, don't want to prune less than that
     if [ $snapCount -lt 3 ] ; then
        return;
     fi
     MAXBE=`expr $snapCount - 1`
  fi

  # If we have less BE than MAX, return
  if [ $snapCount -lt $MAXBE ] ; then return ; fi

  # Reverse the list
  for tmp in $snapList
  do
     rSnaps="$tmp $rSnaps"
  done

  # Do any pruning
  KEEP="$MAXBE"
  num=0
  for snap in $rSnaps
  do
     num=`expr $num + 1`
     # Make sure this BE isn't mounted or running
     cat $bList | grep "^$snap " | grep -q -e " N " -e " NR "  -e " /"
     if [ $? -eq 0 ] ; then continue ; fi

     if [ $num -gt $KEEP ] ; then
        # Remove this old BE
        echo "Removing Boot Environment: $snap"
        beadm destroy -F $snap >/dev/null 2>/dev/null
     fi
  done

  rm $bList
}

# Function to take a gptid/<foo> string, and map it to the real device name
map_gptid_to_dev()
{
  # First check glabel
  local needle="`echo $1 | sed 's|.eli||g'`"
  local realName="`glabel status | grep -w -e $needle | awk '{print $3}'`"
  if [ -n "$realName" ] ; then
     echo "$realName"
     return 0
  fi

  # Do it the hard way
  gpart list > /tmp/.gptList.$$
  # Strip off the gptid/
  needle="`echo $1 | sed 's|gptid/||g'`"
  local realName=""

  while read uline
  do
    echo "$uline" | grep -q " Name: "
    if [ $? -eq 0 ]; then
      realName="`echo $uline | awk '{print $3}'`"
      continue
    fi

    echo "$uline" | grep -q "rawuuid: $needle"
    if [ $? -eq 0 ]; then
       echo "$realName"
       rm /tmp/.gptList.$$
       return 0
       break
    fi
  done < /tmp/.gptList.$$
  rm /tmp/.gptList.$$
  return 1
}

# Find FreeBSD partitions geom name
map_diskid_to_dev()
{
  # Remove the .eli / s1 / p2 or whatever from end of label
  diskID=`echo $1 | sed 's|.eli||g' | rev | cut -c 3- | rev`

  devName="`glabel status | grep -w -e $diskID | awk '{print $3}'`"
  if [ -n "$devName" ] ; then
     echo "${devName}"
  fi
}

# Restamp grub-install onto the ZFS root disks
update_grub_boot()
{
  # Bail out if GRUB isn't setup as the default BL on this system
  if [ ! -e "/boot/grub/grub.cfg" ] ; then return 0; fi

  ROOTFS=`mount | awk '/ \/ / {print $1}'`
  BEDS="$( echo ${ROOTFS} | awk -F '/' '{print $2}' )"
  if [ "$BEDS" = "dev" ] ; then BEDS="ROOT"; fi

  # Check if we can re-stamp the boot-loader on any of this pools disks
  TANK=`echo $ROOTFS | cut -d '/' -f 1`
  zpool status $TANK > /tmp/.zpStatus.$$

  restampDisks=""

  while read zline
  do
     # If we have reached cache / log devices, we can break now
     echo $zline | grep -qw "cache"
     if [ $? -eq 0 ] ; then break ; fi
     echo $zline | grep -qw "log"
     if [ $? -eq 0 ] ; then break ; fi

     # Only try to stamp disks marked as online
     echo $zline | grep -q "state: "
     if [ $? -eq 0 ] ; then continue ; fi
     echo $zline | grep -q "ONLINE"
     if [ $? -ne 0 ] ; then continue ; fi

     # Get the disk name
     disk="`echo $zline | awk '{print $1}'`"

     # Is this a legit disk?
     if [ ! -e "/dev/${disk}" ] ; then continue; fi

     restampDisks="$restampDisks $disk"
  done < /tmp/.zpStatus.$$
  rm /tmp/.zpStatus.$$

  for i in $restampDisks
  do
     disk="$i"

     # If this is a GPTID / rawuuid, find out
     echo "$disk" | grep -q "gptid"
     if [ $? -eq 0 ] ; then
        # Just a GPTID, resolve it down to real device
        disk="$(map_gptid_to_dev ${i})"
        if [ -z "$disk" ] ; then
           echo "Warning: Unable to map ${i} to real device name"
           continue
        fi
     fi

     # Check for diskid glabel
     echo "$disk" | grep -q "diskid"
     if [ $? -eq 0 ] ; then
        # Try to determine disk ID -> adaX mapping
        disk=$(map_diskid_to_dev $disk)
        if [ -z "$disk" ] ; then
           echo "Warning: Unable to map ${i} to real device name"
           continue
        fi
     fi

     # If we are doing a EFI boot
     if [ "`kenv grub.platform 2>/dev/null`" = "efi" ] ; then
        GRUBFLAGS="$GRUBFLAGS --efi-directory=/boot/efi --removable --target=x86_64-efi"
     fi

     # Check encryption
     zpool status | grep -q '\.eli '
     if [ $? -eq 0 ] ; then
       # Check for crypto disk
       cat /usr/local/etc/default/grub | grep -q "GRUB_ENABLE_CRYPTODISK"
       if [ $? -ne 0 ] ; then
         echo "GRUB_ENABLE_CRYPTODISK=y" >> /usr/local/etc/default/grub
       fi
     fi

     # Remove the .eli, if it exists
     disk=`echo $disk | sed 's|.eli||g'`

     # Now get the root of the disk
     disk=`echo $disk | sed 's|p[1-9]$||g' | sed "s|s[1-9][a-z]$||g" | sed "s|s[1-9]]$||g"`
     if [ ! -e "/dev/${disk}" ] ; then continue; fi

     # Re-install GRUB on this disk
     echo "Installing GRUB to $disk" >&2
     grub-install $GRUBFLAGS /dev/${disk} 2>/dev/null
     if [ $? -ne 0 ] ; then
       grub-install /dev/${disk}
     fi
  done

  # Do the copy of config / modules after we run grub-install, which may update modules
  for i in `beadm list -a 2>/dev/null | grep "/${BEDS}/" | awk '{print $1}'`
  do
    if mount | grep -q "$i on / ("; then
       continue
    fi
    echo -e "Copying grub.cfg to $i...\c" >&2
    fMnt="/mnt.$$"
    mkdir $fMnt
    if ! mount -t zfs ${i} $fMnt ; then
       echo "WARNING: Failed to update grub.cfg on: ${i}" >&2
       continue
    else
       # Copy grub config and modules over to old dataset
       # This is done so that newer grub on boot-sector has
       # matching modules to load from all BE's
       cp /boot/grub/grub.cfg ${fMnt}/boot/grub/grub.cfg
       if [ -d "/boot/grub/i386-pc" ] ; then
         rm -rf ${fMnt}/boot/grub/i386-pc
         cp -r /boot/grub/i386-pc ${fMnt}/boot/grub/
       fi
       if [ -d "/boot/grub/x86_64-efi" ] ; then
         rm -rf ${fMnt}/boot/grub/x86_64-efi
         cp -r /boot/grub/x86_64-efi ${fMnt}/boot/grub/
       fi
       echo -e "done" >&2
       umount -f ${fMnt} 2>/dev/null
    fi
    rmdir ${fMnt} 2>/dev/null
  done

  return 0
}

get_root_pool_free_space()
{
  local ROOTPOOL=`mount | grep 'on / ' | cut -d '/' -f 1`
  if [ -z "$ROOTPOOL" ] ; then return ; fi
  local freeSpace=`zpool list -Hp ${ROOTPOOL} | awk '{print $4}'`
  if [ ! $(is_num "$freeSpace") ] ; then return ; fi
  # Convert freespace to Kb
  freeSpace=`expr $freeSpace / 1024 2>/dev/null`
  echo "$freeSpace"
}
