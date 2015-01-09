#!/bin/sh
#-
# Copyright (c) 2010 iXsystems, Inc.  All rights reserved.
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
# $FreeBSD: head/usr.sbin/pc-sysinstall/backend/functions-bsdlabel.sh 247735 2013-03-03 23:07:27Z jpaetzel $

# Functions related to disk operations using bsdlabel

# Check if we are are provided a geli password on the nextline of the config
check_for_enc_pass()
{
  CURLINE="${1}"
 
  get_next_cfg_line "${CFGF}" "${CURLINE}" 
  echo ${VAL} | grep -q "^encpass=" 2>/dev/null
  if [ $? -eq 0 ] ; then
    # Found a password, return it
    get_value_from_string "${VAL}"
    return
  fi

  export VAL=""
  return
};

# On check on the disk-label line if we have any extra vars for this device
get_fs_line_xvars()
{
  ACTIVEDEV="${1}"
  LINE="${2}"

  echo $LINE | cut -d ' ' -f 4 | grep -q '(' 2>/dev/null
  if [ $? -ne 0 ] ; then return ; fi

  # See if we are looking for ZFS specific options
  echo $LINE | grep -q '^ZFS' 2>/dev/null
  if [ $? -eq 0 ] ; then
    ZTYPE="NONE"
    ZFSVARS="`echo $LINE | cut -d ' ' -f 4-20 |cut -d '(' -f 2- | cut -d ')' -f 1 | xargs`"

    echo $ZFSVARS | grep -qE "^(disk|file|mirror|raidz(1|2|3)?|spare|log|cache):" 2>/dev/null
    if [ $? -eq 0 ] ; then
       ZTYPE=`echo $ZFSVARS | cut -f1 -d:`
       tmpVars=`echo $ZFSVARS | sed "s|$ZTYPE: ||g" | sed "s|$ZTYPE:||g"`
       ZFSVARS=""
       # make sure we have a '/dev' in front of the extra devices
       for i in $tmpVars
       do
          echo $i | grep -q '/dev/'
          if [ $? -ne 0 ] ; then
             ZFSVARS="$ZFSVARS /dev/${i}"
          else
             ZFSVARS="$ZFSVARS $i"
          fi
       done
    fi

    # Return the ZFS options
    if [ "${ZTYPE}" = "NONE" ] ; then
      VAR="${ACTIVEDEV} ${ZFSVARS}"
    else
      VAR="${ZTYPE} ${ACTIVEDEV} ${ZFSVARS}"
    fi
    export VAR
    return
  fi # End of ZFS block

  # See if we are looking for UFS specific newfs options
  echo $LINE | grep -q '^UFS' 2>/dev/null
  if [ $? -eq 0 ] ; then
    FSVARS="`echo $LINE | cut -d '(' -f 2- | cut -d ')' -f 1 | xargs`"
    VAR="${FSVARS}"
    export VAR
    return
  fi

  # If we got here, set VAR to empty and export
  export VAR=""
  return
};

# Init each zfs mirror disk with a boot sector so we can failover
setup_zfs_mirror_parts()
{
  _nZFS=""
  SOUT="$4"
  ENC="$5"

  # Check if the target disk is using GRUB
  grep -q "$3" ${TMPDIR}/.grub-install 2>/dev/null
  if [ $? -eq 0 ] ; then
     local _tBL="GRUB"
  else
     local _tBL="bsd"
  fi

  ZTYPE="`echo ${1} | awk '{print $1}'`"

  # Using mirroring, setup boot partitions on each disk
  _mirrline="`echo ${1} | sed 's|mirror ||g' | sed 's|raidz1 ||g' | sed 's|raidz2 ||g' | sed 's|raidz3 ||g' | sed 's|raidz ||g'`"
  for _zvars in $_mirrline
  do
    echo "Looping through _zvars: $_zvars" >>${LOGOUT}
    echo "$_zvars" | grep -q "${2}" 2>/dev/null
    if [ $? -eq 0 ] ; then continue ; fi
    if [ -z "$_zvars" ] ; then continue ; fi

    is_disk "$_zvars" >/dev/null 2>/dev/null
    if [ $? -eq 0 ] ; then

      # Save this disk as one we want to clone the original disk setup to
      ZFS_CLONE_DISKS="$ZFS_CLONE_DISKS ${_zvars}"
      export ZFS_CLONE_DISKS

      echo "Setting up ZFS disk $_zvars" >>${LOGOUT}
      init_gpt_full_disk "$_zvars" "$_tBL" >/dev/null 2>/dev/null

      # If we are not using GRUB we need to add pmbr / gptzfsboot
      if [ "$_tBL" != "GRUB" ] ; then
        rc_halt "gpart bootcode -b /boot/pmbr -p /boot/gptzfsboot -i 1 ${_zvars}" >/dev/null 2>/dev/null
      fi
      # If GELI is enabled
      if [ "$ENC" = "ON" ] ; then
        _nZFS="$_nZFS ${_zvars}p2.eli"
      else
        _nZFS="$_nZFS ${_zvars}p2"
      fi
    else
      _nZFS="$_nZFS ${_zvars}"
    fi	

  done

  # Export the ZXTRAOPTS
  # If GELI is enabled
  if [ "$ENC" = "ON" ] ; then
    export ZXTRAOPTS="$ZTYPE ${2}.eli `echo $_nZFS | tr -s ' '`"
  else
    export ZXTRAOPTS="$ZTYPE $2 `echo $_nZFS | tr -s ' '`"
  fi
} ;

# Function which creates a unique label name for the specified mount
gen_glabel_name()
{
  MOUNT="$1"
  TYPE="$2"
  NUM="0"
  MAXNUM="20"

  if [ "$TYPE" = "ZFS" ] ; then
    NAME="zpool"
  elif [ "$MOUNT" = "/" ] ; then
    NAME="rootfs"
  else
    # If doing a swap partition, also rename it
    if [ "${TYPE}" = "SWAP" ]
    then
      NAME="swap"
    else
      NAME="`echo $MOUNT | sed 's|/||g' | sed 's| ||g'`"
    fi
  fi

  # Loop through and break when we find our first available label
  while
  Z=1
  do
    glabel status | grep -q "${NAME}${NUM}" 2>/dev/null
    if [ $? -ne 0 ]
    then
      break
    else
        NUM=$((NUM+1))
    fi

    if [ $NUM -gt $MAXNUM ]
    then
      exit_err "Cannot allocate additional glabel name for $NAME"
      break
    fi
  done 
   

  export VAL="${NAME}${NUM}" 
};

# Function to determine the size we can safely use when 0 is specified
get_autosize()
{
  # Disk tag to look for
  dTag="$1"

  # Total MB Avail
  get_disk_mediasize_mb "$2"
  local _aSize=$VAL

  while read aline
  do
    # Check for data on this slice
    echo $aline | grep -q "^${_dTag}-part=" 2>/dev/null
    if [ $? -ne 0 ] ; then continue ; fi

    get_value_from_string "${aline}"
    ASTRING="$VAL"

    # Get the size of this partition
    SIZE=`echo $ASTRING | tr -s '\t' ' ' | cut -d ' ' -f 2`
    if [ $SIZE -eq 0 ] ; then continue ; fi
    _aSize=`expr $_aSize - $SIZE`
  done <${CFGF}

  # Pad the size a bit
  _aSize=`expr $_aSize - 2`

  VAL="$_aSize"
  export VAL
};

# Function to setup partitions using gpart
new_gpart_partitions()
{
  local _dTag="$1"
  local _pDisk="$2"
  local _wSlice="$3"
  local _sNum="$4"
  local _pType="$5"
  FOUNDPARTS="1"
  USEDAUTOSIZE=0

  # Lets read in the config file now and setup our partitions
  if [ "${_pType}" = "gpt" ] ; then
    CURPART="2"
  elif [ "${_pType}" = "apm" ] ; then
    CURPART="3"
  else
    PARTLETTER="a"
    CURPART="1"
    if [ "${_pType}" = "mbr" ] ; then
      rc_halt "gpart create -s BSD ${_wSlice}"
    fi
  fi

  # Check if the target disk is using GRUB
  grep -q "$3" ${TMPDIR}/.grub-install 2>/dev/null
  if [ $? -eq 0 ] ; then
     local _tBL="GRUB"
  else
     local _tBL="bsd"
  fi

  # Unset ZFS_CLONE_DISKS
  #ZFS_CLONE_DISKS=""

  while read line
  do
    # Check for data on this slice
    echo $line | grep -q "^${_dTag}-part=" 2>/dev/null
    if [ $? -eq 0 ]
    then
      FOUNDPARTS="0"
      # Found a slice- entry, lets get the slice info
      get_value_from_string "${line}"
      STRING="$VAL"

      # We need to split up the string now, and pick out the variables
      FS=`echo $STRING | tr -s '\t' ' ' | cut -d ' ' -f 1` 
      SIZE=`echo $STRING | tr -s '\t' ' ' | cut -d ' ' -f 2` 
      MNT=`echo $STRING | tr -s '\t' ' ' | cut -d ' ' -f 3` 

      # Check if we have a .eli extension on this FS
      echo ${FS} | grep -q ".eli" 2>/dev/null
      if [ $? -eq 0 ]
      then
        FS="`echo ${FS} | cut -d '.' -f 1`"
        ENC="ON"
        check_for_enc_pass "${line}"
        if [ "${VAL}" != "" ] ; then
          # We have a user supplied password, save it for later
          ENCPASS="${VAL}" 
        fi
      else
        ENC="OFF"
      fi

      # Check if the user tried to setup / as an encrypted partition
      check_for_mount "${MNT}" "/"
      if [ $? -eq 0 -a "${ENC}" = "ON" ]
      then
        export USINGENCROOT="0"
      fi
          
      # Now check that these values are sane
      case $FS in
        UFS|UFS+S|UFS+J|UFS+SUJ|ZFS|SWAP) ;;
       *) exit_err "ERROR: Invalid file system specified on $line" ;;
      esac

      # Check that we have a valid size number
      expr $SIZE + 1 >/dev/null 2>/dev/null
      if [ $? -ne 0 ]; then
        exit_err "ERROR: The size specified on $line is invalid"
      fi

      # Check that the mount-point starts with /
      echo "$MNT" | grep -qe "^/" -e "^none" 2>/dev/null
      if [ $? -ne 0 ]; then
        exit_err "ERROR: The mount-point specified on $line is invalid"
      fi

      if [ "$SIZE" = "0" ]
      then
	if [ $USEDAUTOSIZE -eq 1 ] ; then
          exit_err "ERROR: You can not have two partitions with a size of 0 specified!"
	fi
        case ${_pType} in
	  gpt|apm) get_autosize "${_dTag}" "$_pDisk" ;;
	        *) get_autosize "${_dTag}" "$_wSlice" ;;
        esac
        SOUT="-s ${VAL}M"
	USEDAUTOSIZE=1
      else
        SOUT="-s ${SIZE}M"
      fi

      # Check if we found a valid root partition
      check_for_mount "${MNT}" "/"
      if [ $? -eq 0 ] ; then
        export FOUNDROOT="1"
        if [ "${CURPART}" = "2" -a "$_pType" = "gpt" ] ; then
          export FOUNDROOT="0"
        fi
        if [ "${CURPART}" = "3" -a "$_pType" = "apm" ] ; then
          export FOUNDROOT="0"
        fi
        if [ "${CURPART}" = "1" -a "$_pType" = "mbr" ] ; then
          export FOUNDROOT="0"
        fi
        if [ "${CURPART}" = "1" -a "$_pType" = "gptslice" ] ; then
          export FOUNDROOT="0"
        fi
      fi

      check_for_mount "${MNT}" "/boot"
      if [ $? -eq 0 ] ; then
        export USINGBOOTPART="0"
        if [ "${CURPART}" != "2" -a "${_pType}" = "gpt" ] ; then
            exit_err "/boot partition must be first partition"
        fi
        if [ "${CURPART}" != "3" -a "${_pType}" = "apm" ] ; then
            exit_err "/boot partition must be first partition"
        fi
        if [ "${CURPART}" != "1" -a "${_pType}" = "mbr" ] ; then
            exit_err "/boot partition must be first partition"
        fi
        if [ "${CURPART}" != "1" -a "${_pType}" = "gptslice" ] ; then
            exit_err "/boot partition must be first partition"
        fi

        if [ "${FS}" != "UFS" -a "${FS}" != "UFS+S" -a "${FS}" != "UFS+J" -a "${FS}" != "UFS+SUJ" ] ; then
          exit_err "/boot partition must be formatted with UFS"
        fi
      fi

      # Generate a unique label name for this mount
      gen_glabel_name "${MNT}" "${FS}"
      PLABEL="${VAL}"

      # Get any extra options for this fs / line
      if [ "${_pType}" = "gpt" ] ; then
        get_fs_line_xvars "${_pDisk}p${CURPART}" "${STRING}"
      elif [ "${_pType}" = "apm" ] ; then
        get_fs_line_xvars "${_pDisk}s${CURPART}" "${STRING}"
      else
        get_fs_line_xvars "${_wSlice}${PARTLETTER}" "${STRING}"
      fi
      XTRAOPTS="$VAR"

      # Check if using zfs mirror
      echo ${XTRAOPTS} | grep -q -e "mirror" -e "raidz"
      if [ $? -eq 0 -a "$FS" = "ZFS" ] ; then
        if [ "${_pType}" = "gpt" -o "${_pType}" = "gptslice" ] ; then
	  setup_zfs_mirror_parts "${XTRAOPTS}" "${_pDisk}p${CURPART}" "${_pDisk}" "${SOUT}" "$ENC"
       	  XTRAOPTS="${ZXTRAOPTS}"
        elif [ "${_pType}" = "apm" ] ; then
	  setup_zfs_mirror_parts "${XTRAOPTS}" "${_pDisk}s${CURPART}" "${_pDisk}" "${SOUT}" "$ENC"
       	  XTRAOPTS="${ZXTRAOPTS}"
        else
	  setup_zfs_mirror_parts "${XTRAOPTS}" "${_wSlice}${PARTLETTER}" "${_pDisk}" "${SOUT}" "$ENC"
       	  XTRAOPTS="${ZXTRAOPTS}"
        fi
      fi

      # Figure out the gpart type to use
      case ${FS} in
        ZFS) PARTYPE="freebsd-zfs" ;;
        SWAP) PARTYPE="freebsd-swap" ;;
        *) PARTYPE="freebsd-ufs" ;;
      esac

      # Create the partition
      if [ "${_pType}" = "gpt" ] ; then
        sleep 2
        if [ "${INSTALLTYPE}" = "GhostBSD" ]
        then
          aCmd="gpart add ${SOUT} -t ${PARTYPE} ${_pDisk}"
        else
	  aCmd="gpart add -a 4k ${SOUT} -t ${PARTYPE} ${_pDisk}"
	fi
      elif [ "${_pType}" = "gptslice" ]; then
        sleep 2
        aCmd="gpart add ${SOUT} -t ${PARTYPE} ${_wSlice}"
      elif [ "${_pType}" = "apm" ]; then
        sleep 2
        aCmd="gpart add ${SOUT} -t ${PARTYPE} ${_pDisk}"
      else
        sleep 2
	# MBR type
        aCmd="gpart add ${SOUT} -t ${PARTYPE} -i ${CURPART} ${_wSlice}"
      fi

      # Run the gpart add command now
      rc_halt "$aCmd"

      # Check if we need to clone this layout to a ZFS mirror/raidz disk
      if [ -n "$ZFS_CLONE_DISKS" ] ; then
         for zC in $ZFS_CLONE_DISKS
         do
	    echo_log "Cloning disk layout to ZFS disk ${zC}"
	    rc_halt "gpart add -a 4k ${SOUT} -t ${PARTYPE} ${zC}"
            if [ "$ENC" = "ON" -a "$PARTYPE" = "freebsd-zfs" ] ; then
	       export GELI_CLONE_ZFS_DEV="${_pDisk}p${CURPART}"
	       export GELI_CLONE_ZFS_DISKS="$GELI_CLONE_ZFS_DISKS ${zC}p${CURPART}"
            fi
	    if [ "$PARTYPE" = "freebsd-swap" ] ; then
	       # If this is the first device, save the original swap dev
	       if [ -z "$ZFS_SWAP_DEVS" ] ; then
		  ZFS_SWAP_DEVS="${_pDisk}p${CURPART}"
	       fi
	       # Save this swap device, we will gmirror it later
	       ZFS_SWAP_DEVS="${ZFS_SWAP_DEVS} ${zC}p${CURPART}"
	       export ZFS_SWAP_DEVS
	    fi
         done
      fi

      # Check if this is a root / boot partition, and stamp the right loader
      for TESTMNT in `echo ${MNT} | sed 's|,| |g'`
      do
        if [ "${TESTMNT}" = "/" -a -z "${BOOTTYPE}" ] ; then
           BOOTTYPE="${PARTYPE}" 
        fi 
        if [ "${TESTMNT}" = "/boot" ]  ; then
           BOOTTYPE="${PARTYPE}" 
        fi 
      done 

      # Save this data to our partition config dir
      if [ "${_pType}" = "gpt" ] ; then
	_dFile="`echo $_pDisk | sed 's|/|-|g'`"
        echo "${FS}#${MNT}#${ENC}#${PLABEL}#GPT#${XTRAOPTS}" >${PARTDIR}/${_dFile}p${CURPART}

        # Clear out any headers
        sleep 2
        dd if=/dev/zero of=${_pDisk}p${CURPART} count=2048 2>/dev/null

        # If we have a enc password, save it as well
        if [ -n "${ENCPASS}" ] ; then
          echo "${ENCPASS}" >${PARTDIR}-enc/${_dFile}p${CURPART}-encpass
        fi
      elif [ "${_pType}" = "apm" ] ; then
	_dFile="`echo $_pDisk | sed 's|/|-|g'`"
        echo "${FS}#${MNT}#${ENC}#${PLABEL}#GPT#${XTRAOPTS}" >${PARTDIR}/${_dFile}s${CURPART}

        # Clear out any headers
        sleep 2
        dd if=/dev/zero of=${_pDisk}s${CURPART} count=2048 2>/dev/null

        # If we have a enc password, save it as well
        if [ -n "${ENCPASS}" ] ; then
          echo "${ENCPASS}" >${PARTDIR}-enc/${_dFile}s${CURPART}-encpass
        fi
      else
	# MBR Partition or GPT slice
	_dFile="`echo $_wSlice | sed 's|/|-|g'`"
        echo "${FS}#${MNT}#${ENC}#${PLABEL}#MBR#${XTRAOPTS}#${IMAGE}" >${PARTDIR}/${_dFile}${PARTLETTER}
        # Clear out any headers
        sleep 2
        dd if=/dev/zero of=${_wSlice}${PARTLETTER} count=2048 2>/dev/null

        # If we have a enc password, save it as well
        if [ -n "${ENCPASS}" ] ; then
          echo "${ENCPASS}" >${PARTDIR}-enc/${_dFile}${PARTLETTER}-encpass
        fi
      fi


      # Increment our parts counter
      if [ "$_pType" = "gpt" -o "$_pType" = "apm" ] ; then 
          CURPART=$((CURPART+1))
        # If this is a gpt/apm partition, 
        # we can continue and skip the MBR part letter stuff
        continue
      else
          CURPART=$((CURPART+1))
        if [ "$CURPART" = "3" ] ; then CURPART="4" ; fi
      fi


      # This partition letter is used, get the next one
      case ${PARTLETTER} in
        a) PARTLETTER="b" ;;
        b) PARTLETTER="d" ;;
        d) PARTLETTER="e" ;;
        e) PARTLETTER="f" ;;
        f) PARTLETTER="g" ;;
        g) PARTLETTER="h" ;;
        h) PARTLETTER="ERR" ;;
        *) exit_err "ERROR: bsdlabel only supports up to letter h for partitions." ;;
      esac

    fi # End of subsection locating a slice in config

    echo $line | grep -q "^commitDiskLabel" 2>/dev/null
    if [ $? -eq 0 -a "${FOUNDPARTS}" = "0" ]
    then

      # If this is the boot disk, stamp the right gptboot
      if [ ! -z "${BOOTTYPE}" -a "$_pType" = "gpt" -a "$_tBL" != "GRUB" ] ; then
        case ${BOOTTYPE} in
          freebsd-ufs) rc_halt "gpart bootcode -p /boot/gptboot -i 1 ${_pDisk}" ;;
          freebsd-zfs) rc_halt "gpart bootcode -p /boot/gptzfsboot -i 1 ${_pDisk}" ;;
        esac 
      fi

      # Make sure to stamp the MBR loader
      if [ "$_pType" = "mbr" -a "$_tBL" != "GRUB" ] ; then
	rc_halt "gpart bootcode -b /boot/boot ${_wSlice}"
      fi

      # Found our flag to commit this label setup, check that we found at least 1 partition
      if [ "${CURPART}" = "1" ] ; then
        exit_err "ERROR: commitDiskLabel was called without any partition entries for it!"
      fi

      break
    fi
  done <${CFGF}
};

# Function to modify partitions using gpart
modify_gpart_partitions()
{
  local _dTag="$1"
  local _pDisk="$2"
  local _wSlice="$3"
  local _sNum="$4"
  local _pType="$5"
  FOUNDPARTS="1"

  # Lets read in the config file now and setup our partitions
  if [ "${_pType}" != "gpt" ] ; then
    exit_err "Modification only supports GPT partitions at this time..."
  fi

  # Read through config, lets see what partition we are converting over
  while read line
  do
    # Check if we are at the end
    echo $line | grep -q "^commitDiskLabel" 2>/dev/null
    if [ $? -eq 0 -a "${FOUNDPARTS}" = "0" ]; then
      # Found our flag to commit this label setup, check that we found at least 1 partition
      if [ "${FOUNDPARTS}" = "1" ] ; then
        exit_err "ERROR: commitDiskLabel was called without any partition entries for it!"
      fi
      break
    fi

    # Check for data on this slice
    echo $line | grep -q "^${_dTag}-part=" 2>/dev/null
    if [ $? -ne 0 ]; then continue ; fi

    # We found a partition, lets set it up now
    FOUNDPARTS="0"

    # Found a slice- entry, lets get the slice info
    get_value_from_string "${line}"
    STRING="$VAL"

    # We need to split up the string now, and pick out the variables
    # Size is unused, since we are modifying an existing partition
    FS=`echo $STRING | tr -s '\t' ' ' | cut -d ' ' -f 1`
    SIZE=`echo $STRING | tr -s '\t' ' ' | cut -d ' ' -f 2`
    MNT=`echo $STRING | tr -s '\t' ' ' | cut -d ' ' -f 3`

    # Check if we have a .eli extension on this FS
    echo ${FS} | grep -q ".eli" 2>/dev/null
    if [ $? -eq 0 ]; then
      FS="`echo ${FS} | cut -d '.' -f 1`"
      ENC="ON"
      check_for_enc_pass "${line}"
      if [ "${VAL}" != "" ] ; then
        # We have a user supplied password, save it for later
        ENCPASS="${VAL}"
      fi
    else
      ENC="OFF"
    fi

    # Check if the user tried to setup / as an encrypted partition
    check_for_mount "${MNT}" "/"
    if [ $? -eq 0 -a "${ENC}" = "ON" ]; then
      export USINGENCROOT="0"
    fi
          
    # Now check that these values are sane
    case $FS in
      UFS|UFS+S|UFS+J|UFS+SUJ|ZFS|SWAP) ;;
     *) exit_err "ERROR: Invalid file system specified on $line" ;;
    esac

    # Check that the mount-point starts with /
    echo "$MNT" | grep -qe "^/" -e "^none" 2>/dev/null
    if [ $? -ne 0 ]; then
      exit_err "ERROR: The mount-point specified on $line is invalid"
    fi

    # Check if we found a valid root partition
    check_for_mount "${MNT}" "/"
    if [ $? -eq 0 ] ; then
      export FOUNDROOT="0"
    fi

    # Generate a unique label name for this mount
    gen_glabel_name "${MNT}" "${FS}"
    PLABEL="${VAL}"

    # Get any extra options for this fs / line
    get_fs_line_xvars "${_pDisk}p${_sNum}" "${STRING}"
    XTRAOPTS="$VAR"

    # Figure out the gpart type to use
    case ${FS} in
      ZFS) PARTYPE="freebsd-zfs" ;;
      SWAP) PARTYPE="freebsd-swap" ;;
      *) PARTYPE="freebsd-ufs" ;;
    esac

    # Modify the partition
    aCmd="gpart modify -i ${_sNum} -t ${PARTYPE} ${_pDisk}"

    # Run the gpart modify command now
    rc_halt "$aCmd"

    # Save this data to our partition config dir
    _dFile="`echo $_pDisk | sed 's|/|-|g'`"
    echo "${FS}#${MNT}#${ENC}#${PLABEL}#GPT#${XTRAOPTS}" >${PARTDIR}/${_dFile}p${_sNum}

    # Clear out any headers
    sleep 2
    dd if=/dev/zero of=${_pDisk}p${CURPART} count=2048 2>/dev/null

    # If we have a enc password, save it as well
    if [ -n "${ENCPASS}" ] ; then
      echo "${ENCPASS}" >${PARTDIR}-enc/${_dFile}p${_sNum}-encpass
    fi
  done <${CFGF}
};

# Reads through the config and sets up a BSDLabel for the given slice
populate_disk_label()
{
  if [ -z "${1}" ]
  then
    exit_err "ERROR: populate_disk_label() called without argument!"
  fi

  # Set some vars from the given working slice
  diskid="`echo $1 | cut -d ':' -f 1`" 
  disk="`echo $1 | cut -d ':' -f 1 | sed 's|-|/|g'`" 
  slicenum="`echo $1 | cut -d ':' -f 2`" 
  type="`echo $1 | cut -d ':' -f 3`" 
  mod="`echo $1 | cut -d ':' -f 4`"

  # Check if we are only modifying an existing GPT partition
  MODONLY="NO"
  if [ "$mod" = "mod" ] ; then MODONLY="YES"; fi
  
  # Set WRKSLICE based upon format we are using
  if [ "$type" = "mbr" ] ; then
    wrkslice="${diskid}s${slicenum}"
  fi
  if [ "$type" = "apm" ] ; then
    wrkslice="${diskid}s${slicenum}"
  fi
  if [ "$type" = "gpt" -o "$type" = "gptslice" ] ; then
    wrkslice="${diskid}p${slicenum}"
  fi

  if [ ! -e "${SLICECFGDIR}/${wrkslice}" ] ; then
    exit_err "ERROR: Missing SLICETAG data. This shouldn't happen - please let the developers know"
  fi

  disktag="`cat ${SLICECFGDIR}/${wrkslice}`"
  slicedev="`echo $wrkslice | sed 's|-|/|g'`"
  
  if [ "$MODONLY" = "YES" ] ; then
    # We are doing a modification of GPT partitions
    modify_gpart_partitions "${disktag}" "${disk}" "${slicedev}" "${slicenum}" "${type}"
  else
    # Setup the new partitions with gpart
    new_gpart_partitions "${disktag}" "${disk}" "${slicedev}" "${slicenum}" "${type}"
  fi

};

# Function which reads in the disk slice config, and performs it
setup_disk_label()
{
  # We are ready to start setting up the label, lets read the config and do the actions
  # First confirm that we have a valid WORKINGSLICES
  if [ -z "${WORKINGSLICES}" ]; then
    exit_err "ERROR: No slices were setup! Please report this to the maintainers"
  fi

  # Setup some files which we'll be referring to
  export LABELLIST="${TMPDIR}/workingLabels"
  rm $LABELLIST >/dev/null 2>/dev/null

  # Set our flag to determine if we've got a valid root partition in this setup
  export FOUNDROOT="-1"

  # Check if we are using a /boot partition
  export USINGBOOTPART="1"
 
  # Set encryption on root check
  export USINGENCROOT="1"
  
  # Make the tmp directory where we'll store FS info & mount-points
  rm -rf ${PARTDIR} >/dev/null 2>/dev/null
  mkdir -p ${PARTDIR} >/dev/null 2>/dev/null
  rm -rf ${PARTDIR}-enc >/dev/null 2>/dev/null
  mkdir -p ${PARTDIR}-enc >/dev/null 2>/dev/null

  for i in $WORKINGSLICES
  do
    populate_disk_label "${i}"
  done

  # Check if we made a root partition
  if [ "$FOUNDROOT" = "-1" ]
  then
    exit_err "ERROR: No root (/) partition specified!!"
  fi

  # Check if we made a root partition
  if [ "$FOUNDROOT" = "1" -a "${USINGBOOTPART}" != "0" ]
  then
    exit_err "ERROR: (/) partition isn't first partition on disk!"
  fi

};

check_fstab_mbr()
{
  local SLICE
  local FSTAB

  if [ -z "$2" ]
  then
	return 1
  fi

  SLICE="$1"
  FSTAB="$2/etc/fstab"

  if [ -f "${FSTAB}" ]
  then
    PARTLETTER=`echo "$SLICE" | sed -E 's|^.+([a-h])$|\1|'`

    cat "${FSTAB}" | awk '{ print $2 }' | grep -qE '^/$' 2>&1
    if [ $? -eq 0 ]
    then
      if [ "${PARTLETTER}" = "a" ]
      then
        FOUNDROOT="0"
      else
        FOUNDROOT="1"
      fi

      ROOTIMAGE="1"

      export FOUNDROOT
      export ROOTIMAGE
    fi

    cat "${FSTAB}" | awk '{ print $2 }' | grep -qE '^/boot$' 2>&1
    if [ $? -eq 0 ]
    then
      if [ "${PARTLETTER}" = "a" ]
      then
        USINGBOOTPART="0"
      else 
        exit_err "/boot partition must be first partition"
      fi 
      export USINGBOOTPART
    fi

    return 0
  fi

  return 1
};

check_fstab_gpt()
{
  local SLICE
  local FSTAB

  if [ -z "$2" ]
  then
	return 1
  fi

  SLICE="$1"
  FSTAB="$2/etc/fstab"

  if [ -f "${FSTAB}" ]
  then
    PARTNUMBER=`echo "${SLICE}" | sed -E 's|^.+p([0-9]*)$|\1|'`

    cat "${FSTAB}" | awk '{ print $2 }' | grep -qE '^/$' 2>&1
    if [ $? -eq 0 ]
    then
      if [ "${PARTNUMBER}" = "2" ]
      then
        FOUNDROOT="0"
      else
        FOUNDROOT="1"
      fi

      ROOTIMAGE="1"

      export FOUNDROOT
      export ROOTIMAGE
    fi

    cat "${FSTAB}" | awk '{ print $2 }' | grep -qE '^/boot$' 2>&1
    if [ $? -eq 0 ]
    then
      if [ "${PARTNUMBER}" = "2" ]
      then
        USINGBOOTPART="0"
      else 
        exit_err "/boot partition must be first partition"
      fi 
      export USINGBOOTPART
    fi

    return 0
  fi


  return 1
};

check_disk_layout()
{
  local SLICES
  local TYPE
  local DISK
  local RES
  local F

  DISK="$1"
  TYPE="MBR"

  if [ -z "${DISK}" ]
  then
	return 1
  fi

  SLICES_MBR=`ls /dev/${DISK}s[1-4]*[a-h]* 2>/dev/null`
  SLICES_GPT=`ls /dev/${DISK}p[0-9]* 2>/dev/null`
  SLICES_SLICE=`ls /dev/${DISK}[a-h]* 2>/dev/null`

  if [ -n "${SLICES_MBR}" ]
  then
    SLICES="${SLICES_MBR}"
    TYPE="MBR"
    RES=0
  fi
  if [ -n "${SLICES_GPT}" ]
  then
    SLICES="${SLICES_GPT}"
    TYPE="GPT"
    RES=0
  fi
  if [ -n "${SLICES_SLICE}" ]
  then
    SLICES="${SLICES_SLICE}"
    TYPE="MBR"
    RES=0
  fi
  
  for slice in ${SLICES}
  do
    F=1
    mount ${slice} /mnt 2>/dev/null
    if [ $? -ne 0 ]
    then
      continue
    fi 

    if [ "${TYPE}" = "MBR" ]
    then
	  check_fstab_mbr "${slice}" "/mnt"
      F="$?"

    elif [ "${TYPE}" = "GPT" ]
    then
	  check_fstab_gpt "${slice}" "/mnt"
      F="$?"
    fi 

    if [ ${F} -eq 0 ]
    then
      umount /mnt
      break 
    fi

    umount /mnt
  done

  return ${RES}
};
