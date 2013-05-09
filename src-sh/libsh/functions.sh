#!/bin/sh
# Functions we can source for pc-bsd scripts
# Author: Kris Moore
# Copyright: 2012
# License: BSD
##############################################################

PCBSD_ETCCONF="/usr/local/etc/pcbsd.conf"

download_cache_packages()
{
  if [ ! -e "/usr/local/etc/pkg.conf" ] ; then
    exit_err "No /usr/local/etc/pkg.conf!"
  fi

  # Tickle pkg update first
  pkg-static update
  local ARCH="`uname -m`"

  ${1} > /tmp/.pkgUpList.$$

  while read line
  do
     lTag=`echo $line | awk '{print $1}'` 
     case $lTag in
    Upgrading|Downgrading) pkgList="`echo $line | awk '{print $2}' | sed 's|:||g'`-`echo $line | awk '{print $5}'`.txz $pkgList" ;;
 Reinstalling) pkgList="`echo $line | awk '{print $2}'`.txz $pkgList" ;;
   Installing) pkgList="`echo $line | awk '{print $2}' | sed 's|:||g'`-`echo $line | awk '{print $3}'`.txz $pkgList" ;;
                    *) continue ;;
     esac

  done < /tmp/.pkgUpList.$$
  rm /tmp/.pkgUpList.$$

  # Get the PKG_CACHEDIR
  PKG_CACHEDIR="/var/cache/pkg"
  cat /usr/local/etc/pkg.conf | grep -q "^PKG_CACHEDIR:"
  if [ $? -eq 0 ] ; then
    PKG_CACHEDIR="`grep '^PKG_CACHEDIR:' /usr/local/etc/pkg.conf | awk '{print $2}'`"
  fi
  if [ -z "$PKG_CACHEDIR" ] ; then
     exit_err "Failed getting PKG_CACHEDIR"
  fi
  export PKG_CACHEDIR

  PKGREL=`uname -r | cut -d '-' -f 1-2`

  # Where are the packages on our mirrors?
  pkgUrl="/packages/${PKGREL}/${ARCH}"

  if [ ! -d "$PKG_CACHEDIR/All" ] ; then
     mkdir -p ${PKG_CACHEDIR}/All
  fi

  for i in $pkgList
  do
    # Does the package already exist?
    if [ -e "${PKG_CACHEDIR}/All/${i}" ] ; then 
	# For now just remove the cached file
	# Once bapt gives us a working rquery string, we can add a check here to skip
	# re-downloading already valid files
	#pName=`echo $i | sed 's|.txz$||g'`
	# Check the sizes
	#eSize=`pkg rquery "%sb" $pName`
	#dSize=`ls -al `
	rm ${PKG_CACHEDIR}/All/${i} ; 
    fi
    get_file_from_mirrors "${pkgUrl}/All/${i}" "${PKG_CACHEDIR}/All/${i}"
    if [ $? -ne 0 ] ; then
      echo "Failed downloading: /${pkgUrl}/All/${i}"
      return 1
    fi
  done
  return 0
}

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
# with the aria2c command
# Will return just a single mirror, if the user has manually specified one
# in /usr/local/etc/pcbsd.conf
get_aria_mirror_list()
{
  if [ -z $1 ] ; then
     exit_err "Need to supply file to grab from mirrors..."
  fi

  # Set the mirror URL
  local VAL="`cat ${PCBSD_ETCCONF} 2>/dev/null | grep 'PCBSD_MIRROR: ' | sed 's|PCBSD_MIRROR: ||g'`"
  if [ -n "$VAL" ] ; then
     echo "${VAL}${1}"
     return
  fi

  if [ ! -e "/usr/local/share/pcbsd/conf/pcbsd-mirrors" ] ; then
     exit_err "Missing mirror list: /usr/local/share/pcbsd/conf/pcbsd-mirrors"
  fi

  # Build the mirror list
  while read line
  do
    VAL="$VAL ${line}${1}"
  done < /usr/local/share/pcbsd/conf/pcbsd-mirrors
  echo ${VAL}
}

# Function to download a file from the pcbsd mirrors
# Arg1 = Remote File URL
# Arg2 = Where to save file
get_file_from_mirrors()
{
   _rf="${1}"
   _lf="${2}"

   # Get any proxy information
   . /etc/profile

   # Split up the dir / file name
   local aDir=`dirname $_lf`
   local aFile=`basename $_lf`

   # Server status flag
   if [ `id -u` = "0" ] ; then
      aStatFile=/root/.pcbsd-aria-stat
   else
      aStatFile=${HOME}/.pcbsd-aria-stat
   fi

   if [ -e "$aStatFile" ] ; then
     local aStat="--server-stat-of=$aStatFile --server-stat-if=$aStatFile --uri-selector=adaptive --server-stat-timeout=864000"
   else
     local aStat="--server-stat-of=$aStatFile --uri-selector=adaptive"
   fi
   touch $aStatFile

   # Get mirror list
   local mirrorList="$(get_aria_mirror_list $1)"
   
   # Running from a non GUI?
   if [ "$GUI_FETCH_PARSING" != "YES" -a "$PBI_FETCH_PARSING" != "YES" -a -z "$PCFETCHGUI" ] ; then
      aria2c -k 5M ${aStat} --check-certificate=false --file-allocation=none -d "${aDir}" -o "${aFile}" ${mirrorList}
      return $?
   fi

   echo "FETCH: ${_rf}"

   # Doing a front-end download, parse the output of fetch
   _eFile="/tmp/.fetch-exit.$$"
   fetch -s "`echo ${mirrorList} | awk '{print $1}'`" > /tmp/.fetch-size.$$ 2>/dev/null
   _fSize=`cat /tmp/.fetch-size.$$ 2>/dev/null`
   _fSize="`expr ${_fSize} / 1024 2>/dev/null`"
   rm "/tmp/.fetch-size.$$" 2>/dev/null
   _time=1
   if [ -z "$_fSize" ] ; then _fSize=0; fi

   ( aria2c -o ${aFile} -d ${aDir} -k 5M ${aStat} --check-certificate=false --file-allocation=none ${mirrorList} >/dev/null 2>/dev/null ; echo "$?" > ${_eFile} ) &
   FETCH_PID=$!
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
      jobs -l >/tmp/.jobProcess.$$
      cat /tmp/.jobProcess.$$ | awk '{print $3}' | grep -q ${FETCH_PID}
      if [ "$?" != "0" ] ; then rm /tmp/.jobProcess.$$ ; break ; fi
      sleep 1
      _time=`expr $_time + 1`
   done

   _err="`cat ${_eFile}`"
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

		_err="`cat ${_eFile}`"
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

check_pkg_conflicts()
{
  # Lets test if we have any conflicts
  pkg-static ${1} 2>/tmp/.pkgConflicts.$$ >/tmp/.pkgConflicts.$$
  if [ $? -eq 0 ] ; then rm /tmp/.pkgConflicts.$$ ; return ; fi
 
  # Found conflicts, suprise suprise, yet another reason I hate packages
  # Lets start building a list of the old packages we can prompt to remove

  # Nice ugly sed line, sure this can be neater
  cat /tmp/.pkgConflicts.$$ | grep 'WARNING: locally installed' \
	| sed 's|.*installed ||g' | sed 's| conflicts.*||g' | sort | uniq \
	> /tmp/.pkgConflicts.$$.2
  while read line
  do
    cList="$line $cList"
  done < /tmp/.pkgConflicts.$$.2
  rm /tmp/.pkgConflicts.$$.2 
  rm /tmp/.pkgConflicts.$$

  if [ "$GUI_FETCH_PARSING" != "YES" -a "$PBI_FETCH_PARSING" != "YES" -a -z "$PCFETCHGUI" ] ; then
        echo "The following packages will conflict with your pkg command:"
        echo "-------------------------------------"
        echo "$cList" | more
	echo "Do you wish to remove them automatically?"
	echo -e "Default yes: (y/n)\c"
        read tmp
	if [ "$tmp" != "y" -a "$tmp" != "Y" ] ; then return 1 ; fi
  else
	echo "PKGCONFLICTS: $cList"
	echo "PKGREPLY: /tmp/pkgans.$$"
	while : 
        do
	  if [ -e "/tmp/pkgans.$$" ] ; then
	    ans=`cat /tmp/pkgans.$$`
            if [ "$ans" = "yes" ] ; then 
	       break
            else
               return 1
            fi
          fi 
	  sleep 3
	done
  fi

  # Lets auto-resolve these bad-boys
  # Right now the logic is pretty simple, you conflict, you die
  for bPkg in $cList
  do
     # Nuked!
     echo "Removing conflicting package: $bPkg"
     pkg delete -q -y -f ${bPkg}
  done

  # Lets test if we still have any conflicts
  pkg-static ${1} 2>/dev/null >/dev/null
  if [ $? -eq 0 ] ; then return 0; fi

  # Crapola, we still have conflicts, lets warn and bail
  echo "ERROR: pkg ${1} is still reporting conflicts... Resolve these manually and try again"
  return 1
}

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
  mplayer -fs -nomouseinput -zoom /usr/local/share/pcbsd/movies/$mov

  # Setting a language
  if [ -e "/etc/pcbsd-lang" ] ; then
    LANG=`cat /etc/pcbsd-lang`
    export LANG
  fi

  # Start first-boot wizard
  /usr/local/bin/pc-firstboot
  if [ $? -eq 0 ] ; then
    rm /var/.pcbsd-firstgui
  fi
}
