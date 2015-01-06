#!/bin/sh
# License: BSD
# Authors: Joshua Smith (joshms@pcbsd.org) & Kris Moore (kris@pcbsd.org)
########################################################
# This script is fairly linear, it will walk through a series of questions
# and when finished, generate a pc-sysinstall script

# Source our functions
. /usr/local/share/pcbsd/scripts/functions.sh


# Dialog menu title
TITLE="PC-BSD Install Dialog"

# pc-sysinstall config file to write out to
CFGFILE="/tmp/sys-install.cfg"

# Default ZFS layout
ZFSLAYOUT="/(compress=lz4|atime=off),/root(compress=lz4),/tmp(compress=lz4),/usr(canmount=off|mountpoint=none),/usr/home(compress=lz4),/usr/jails(compress=lz4),/usr/obj(compress=lz4),/usr/pbi(compress=lz4),/usr/ports(compress=lz4),/usr/src(compress=lz4),/var(canmount=off|atime=on|mountpoint=none),/var/audit(compress=lz4),/var/log(compress=lz4),/var/tmp(compress=lz4)"

# Ugly master list of settable ZFS properties
ZPROPS="aclinherit(discard|noallow|restricted|passthrough|passthrough-x),aclmode(discard|groupmask|passthrough|restricted),atime(on|off),canmount(on|off|noauto),checksum(on|off|fletcher2|fletcher4|sha256),compress(on|off|lzjb|gzip|zle|lz4),copies(1|2|3),dedup(on|off|verify|sha256),exec(on|off),primarycache(all|none|metadata),readonly(on|off),secondarycache(all|none|metadata),setuid(on|off),sharenfs(on|off),logbias(latency|throughput),snapdir(hidden|visible),sync(standard|always|disabled),jailed(off|on)"

PCSYS="/root/pc-sysinstall/pc-sysinstall"
if [ ! -e "$PCSYS" ] ; then
  PCSYS="/usr/local/sbin/pc-sysinstall"
fi

# The current ZPOOL type should default to single
ZPOOL_TYPE="single"

change_zpool()
{
  get_zpool_menu
  gen_pc-sysinstall_cfg
}

get_zpool_menu()
{
  while :
  do
    dOpts="done \"Exit zpool menu\" single \"Convert to single-disk\""

    diskTot=`${PCSYS} disk-list | wc -l | awk '{print $1}'`
    if [ $diskTot -gt 1 ] ; then
      dOpts="$dOpts mirror \"Convert to mirror\" raidz1 \"Convert to raidz1\""
    fi
    if [ $diskTot -gt 2 ] ; then
      dOpts="$dOpts raidz2 \"Convert to raidz2\""
    fi
    if [ $diskTot -gt 3 ] ; then
      dOpts="$dOpts raidz3 \"Convert to raidz3\""
    fi

    get_dlg_ans "--menu \"Current zpool: $ZPOOL_TYPE - $SYSDISK $ZPOOL_DISKS\" 20 50 10 ${dOpts}"
    if [ -z "$ANS" ] ; then
       exit_err "Invalid option selected!"
    fi
    case $ANS in
       done) break ;;
     single) ZPOOL_DISKS=""
             ZPOOL_TYPE="single"
             ;;
     mirror) get_zpool_disks "mirror" "1" ;;
     raidz1) get_zpool_disks "raidz1" "1" ;;
     raidz2) get_zpool_disks "raidz2" "2" ;;
     raidz3) get_zpool_disks "raidz3" "3" ;;
          *) ;;
    esac
  done
}

get_zpool_disks() {
  local type=$1
  local min=$2

  while :
  do
    dOpts=""

    ${PCSYS} disk-list > /tmp/.dList.$$
    while read i
    do
      # Get the disk dev
      d=`echo $i | cut -d ':' -f 1`

      # Dont need to list the existing target disk
      if [ "$SYSDISK" = "$d" ] ; then continue ; fi

      # Get the disk description
      desc=`echo $i | cut -d ':' -f 2`
      size="`${PCSYS} disk-info $d | grep size | cut -d '=' -f 2`MB"
      dOpts="$dOpts $d \"$desc ($size)\" off"
    done < /tmp/.dList.$$
    rm /tmp/.dList.$$


    get_dlg_ans_no_exit "--single-quoted --checklist \"Select at least $min additional disk(s) for $type\" 22 45 15 ${dOpts}"
    if [ $? -ne 0 ] ; then break; fi

     ANS=`echo $ANS | sed "s|'||g"`

     count=`echo $ANS | wc -w | awk '{print $1}'`
     if [ $count -lt $min ] ; then
        echo "Please select at least $min additional disks!"
	rtn
        continue
     fi

     ZPOOL_DISKS="$ANS"
     ZPOOL_TYPE="$type"
     break
  done
}

change_zfs()
{
  get_zfs_layout
  gen_pc-sysinstall_cfg
}

get_zfs_layout()
{
  while :
  do
    dOpts="done \"Exit dataset menu\" add \"New ZFS dataset\""
    for z in `echo $ZFSLAYOUT | sed 's|,| |g'`
    do
       d=`echo $z | cut -d '(' -f 1`
       echo "$z" | grep -q '('
       if [ $? -eq 0 ] ; then
         desc="(`echo $z | cut -d '(' -f 2`"
       else
         desc=""
       fi
       dOpts="$dOpts $d \"$desc\""
    done
    get_dlg_ans "--menu \"Select dataset to edit\" 22 78 15 ${dOpts}"
    if [ -z "$ANS" ] ; then
       exit_err "Invalid dataset selected!"
    fi
    case $ANS in
       done) break ;;
        add) add_dataset ;;
          *) edit_dataset "$ANS" ;;
    esac
  done
}

get_zfs_dset_opt()
{
  local changeOpt="$1"
  dOpts=""

  # Build a list of dataset options to display
  for z in `echo $ZPROPS | sed 's|,| |g'`
  do
    zOpt=`echo $z | cut -d '(' -f 1`
    if [ "$zOpt" != "$changeOpt" ] ; then continue ; fi

    zOps=`echo $z | cut -d '(' -f 2 | cut -d ')' -f 1`
    for o in `echo $zOps | sed 's,|, ,g'`
    do
       d="$o" 
       desc="($o)"
       dOpts="$dOpts $d \"$desc\""
    done
    break
  done

  # Ask what to do on this dataset
  get_dlg_ans "--menu \"Set option for $changeOpt on $2\" 22 50 15 unset 'Unset this option' cancel 'Cancel' ${dOpts}"
  if [ -z "$ANS" ] ; then
     exit_err "Invalid option selected!"
  fi

  if [ "$ANS" = "unset" ] ; then ANS="" ; fi
  if [ "$ANS" = "cancel" ] ; then ANS="CANCELED" ; fi
  VAL=$ANS ; export VAL
}

edit_dataset()
{

  dOpts=""

  # Pull a list of dataset options from the users layout
  for z in `echo $ZFSLAYOUT | sed 's|,| |g'`
  do
     d=`echo $z | cut -d '(' -f 1`
     if [ "$d" != "$1" ] ; then continue ; fi
     curOpts="`echo $z | cut -d '(' -f 2 | cut -d ')' -f 1`"
  done

  # Build a list of dataset options to display
  for z in `echo $ZPROPS | sed 's|,| |g'`
  do
    d=`echo $z | cut -d '(' -f 1`
    desc="(unset)"
    for o in `echo $curOpts | sed 's,|, ,g'`
    do
       _opt="`echo $o | cut -d '=' -f 1`"
       if [ "$_opt" != "$d" ] ; then continue ; fi
       desc="(`echo $o | cut -d '=' -f 2`)"
    done
    dOpts="$dOpts $d \"$desc\""
  done

  # Ask what to do on this dataset
  get_dlg_ans "--menu \"Editing dataset: ${1}\" 22 50 15 delete 'Remove the dataset' cancel 'Cancel' ${dOpts}"
  if [ -z "$ANS" ] ; then
     exit_err "Invalid dataset selected!"
  fi
  case $ANS in
    cancel) return ;;
    delete) NEWLAYOUT=""
            for z in `echo $ZFSLAYOUT | sed 's|,| |g'`
            do
              d=`echo $z | cut -d '(' -f 1`  
	      if [ "$d" = "$1" ] ; then continue ; fi
	      if [ -z "$NEWLAYOUT" ] ; then
	        NEWLAYOUT="${z}"
              else
	        NEWLAYOUT="$NEWLAYOUT,${z}"
              fi
            done
	    ZFSLAYOUT="$NEWLAYOUT"
            ;;
         *) cOpt=$ANS 
	    get_zfs_dset_opt "$cOpt" "$1"
	    newOpt="$VAL"
	    if [ "$newOpt" = "CANCELED" ] ; then return ; fi

            NEWLAYOUT=""
            for z in `echo $ZFSLAYOUT | sed 's|,| |g'`
            do
               d=`echo $z | cut -d '(' -f 1`
               if [ "$d" != "$1" ] ; then 
                 if [ -z "$NEWLAYOUT" ] ; then
                   NEWLAYOUT="${z}"
                 else
                   NEWLAYOUT="$NEWLAYOUT,${z}"
                 fi
	         continue
	       fi
		
	       # Add this option to our optList
	       NEWOPTLIST=""
	       optList="`echo $z | cut -d '(' -f 2 | cut -d ')' -f 1`"
	       if [ "$optList" = "$z" ] ; then optList="" ; fi
	       addedOpt=0
               for o in `echo $optList | sed 's,|, ,g'`
               do
                  _opt="`echo $o | cut -d '=' -f 1`"
                  if [ "$_opt" != "$cOpt" ] ; then
                     if [ -z "$NEWOPTLIST" ] ; then
                        NEWOPTLIST="${o}"
                     else
                        NEWOPTLIST="$NEWOPTLIST|${o}"
                     fi
	             continue
		  fi
		  addedOpt=1
		  # If the user unset this opt, lets skip adding it
		  if [ -z "$newOpt" ] ; then continue ; fi
                  if [ -z "$NEWOPTLIST" ] ; then
                     NEWOPTLIST="${cOpt}=${newOpt}"
                  else
                     NEWOPTLIST="$NEWOPTLIST|${cOpt}=${newOpt}"
                  fi
               done

	       # If we need to add the opt fresh
	       if [ $addedOpt -eq 0 ] ; then
                  if [ -z "$NEWOPTLIST" ] ; then
                     NEWOPTLIST="${cOpt}=${newOpt}"
                  else
                     NEWOPTLIST="$NEWOPTLIST|${cOpt}=${newOpt}"
                  fi
	       fi

               opt=""
	       if [ -n "$NEWOPTLIST" ] ; then opt="($NEWOPTLIST)"; fi
               if [ -z "$NEWLAYOUT" ] ; then
                  NEWLAYOUT="${d}${opt}"
               else
                  NEWLAYOUT="$NEWLAYOUT,${d}${opt}"
	       fi
            done
            ZFSLAYOUT="$NEWLAYOUT"
            ;;
  esac
}

add_dataset()
{
    get_dlg_ans "--inputbox 'Enter dataset mountpoint' 8 40"
    if [ -z "$ANS" ] ; then
       exit_err "Invalid dataset entered!"
    fi

    # Make sure it starts with a /
    echo $ANS | grep -q "^/" 
    if [ $? -ne 0 ] ; then
       return
    fi

    # Check for duplicates
    for z in `echo $ZFSLAYOUT | sed 's|,| |g'`
    do
       d=`echo $z | cut -d '(' -f 1`
       if [ "$d" = "$ANS" ] ; then
          echo "Error, this dataset already exists!"
          rtn
	  return
       fi
    done

    # Save the dataset
    ZFSLAYOUT="$ZFSLAYOUT,$ANS"
}

get_dlg_ans_no_exit()
{
  TANS="/tmp/.pcinsdialog.$$"
  if [ -e "$TANS" ] ; then rm ${TANS}; fi
  if [ -e "$TANS.dlg" ] ; then rm ${TANS}.dlg; fi
  while :
  do
    echo "dialog --title \"$TITLE\" ${@}" >${TANS}.dlg
    sh ${TANS}.dlg 2>${TANS}
    local err=$?

    if [ ! -e "$TANS" ] ; then
       ANS=""
       return $err
    fi
    ANS=`cat ${TANS}`
    return $err
  done
}

get_dlg_ans()
{
  TANS="/tmp/.pcinsdialog.$$"
  if [ -e "$TANS" ] ; then rm ${TANS}; fi
  if [ -e "$TANS.dlg" ] ; then rm ${TANS}.dlg; fi
  while :
  do
    echo "dialog --title \"$TITLE\" ${@}" >${TANS}.dlg
    sh ${TANS}.dlg 2>${TANS}
    if [ $? -ne 0 ] ; then
      dialog --title "$TITLE" --yesno 'Exit the installer?' 8 30
      if [ $? -eq 0 ] ; then exit_err "User canceled install" ; fi
      continue
    fi

    if [ ! -e "$TANS" ] ; then
       ANS=""
       return
    fi
    ANS=`cat ${TANS}`
    return
  done
}


get_sys_type()
{
  unset SYSTYPE

  # Determine if sys-type is TrueOS or PC-BSD
  if [ -e /usr/local/bin/startx ] ; then
    get_dlg_ans "--radiolist \"System type\" 12 50 5 desktop \"PC-BSD Desktop\" on server \"TrueOS / FreeBSD Server\" off"
    SYSTYPE="$ANS"
  else
    SYSTYPE="server"
  fi
  if [ -z "$SYSTYPE" ] ; then
     exit_err "Invalid system type"
  fi

  # If on a desktop, lets just set DHCP
  if [ "$SYSTYPE" = "desktop" ] ; then
     SYSNIC="AUTO-DHCP-SLAAC"
  fi
}

get_sys_bootmanager()
{
  # Ask the boot-manager
  get_dlg_ans "--radiolist \"Boot Manager\" 12 50 5 GRUB \"GRUB - Recommended\" on none \"No boot-loader\" off"
  if [ -z "$ANS" ] ; then
     exit_err "Invalid bootmanager type"
  fi
  SYSBOOTMANAGER="$ANS"

  # If we are not using grub / gpt, nothing left to ask
  if [ "$SYSBOOTMANAGER" != "GRUB" ]; then return; fi
  if [ "$DISKFORMAT" = "MBR" ]; then return; fi

  # If we are using GRUB, ask if we want to do GELI encryption
  dialog --title "$TITLE" --yesno 'Enable full-disk encryption with GELI?' 8 30
  if [ $? -ne 0 ] ; then return ; fi
  get_dlg_ans "--passwordbox 'Enter encryption password' 8 40"

  if [ -z "$ANS" ] ; then
     echo "No password specified!  GELI encryption is currently disabled." >> /tmp/.GELIinfo.$$
     echo "Please run the wizard again to setup GELI encryption!" >> /tmp/.GELIinfo.$$
     USINGGELI="NO"
     dialog --tailbox /tmp/.GELIinfo.$$ 10 80
     rm /tmp/.GELIinfo.$$
     return
  fi
     
  GELIPASS="$ANS"
  get_dlg_ans "--passwordbox 'Enter password (again)' 8 40"
  if [ -z "$ANS" ] ; then
     echo "No password specified!  GELI encryption is currently disabled." >> /tmp/.GELIinfo.$$
     echo "Please run the wizard again to setup GELI encryption!" >> /tmp/.GELIinfo.$$
     USINGGELI="NO"
     dialog --tailbox /tmp/.GELIinfo.$$ 10 80
     rm /tmp/.GELIinfo.$$
     return
  fi
     
  if [ "$GELIPASS" != "$ANS" ]; then
     echo "ERROR: Password mismatch! GELI encryption is currently disabled." >> /tmp/.GELIinfo.$$
     echo "Please run the wizard again to setup GELI encryption!" >> /tmp/.GELIinfo.$$
     USINGGELI="NO"
     dialog --tailbox /tmp/.GELIinfo.$$ 10 80
     rm /tmp/.GELIinfo.$$
     return
  fi

  USINGGELI="YES"

}

get_target_disk()
{
  # Now we prompt for the disk to install on
  dOpts=""
  ${PCSYS} disk-list > /tmp/.dList.$$
  while read i
  do
     fOpt="on"
     d=`echo $i | cut -d ':' -f 1`
     desc=`echo $i | cut -d ':' -f 2`
     size="`${PCSYS} disk-info $d | grep size | cut -d '=' -f 2`MB"
     dOpts="$dOpts $d \"$desc ($size)\" $fOpt" 
     if [ -z "$fOpt" ] ; then fOpt="off"; fi
  done < /tmp/.dList.$$
  rm /tmp/.dList.$$
  get_dlg_ans "--radiolist \"Select target disk\" 12 50 5 ${dOpts}"
  if [ -z "$ANS" ] ; then
     exit_err "Invalid disk selected!"
  fi
  SYSDISK="$ANS"
}

get_hardware_info()
{

 #detect CPU
   echo "CPU Detected:" >> /tmp/.hardwareinfo.$$
   sysctl -a | egrep -i 'hw.machine|hw.model|hw.ncpu' >> /tmp/.hardwareinfo.$$  
   echo " " >> /tmp/.hardwareinfo.$$
   
 #grep for amount of physical memory and free memory
   echo "Memory Information:" >> /tmp/.hardwareinfo.$$
   grep "real memory" /var/run/dmesg.boot | head -n 1 >> /tmp/.hardwareinfo.$$
   grep "avail memory" /var/run/dmesg.boot | head -n 1 >> /tmp/.hardwareinfo.$$
   echo " " >> /tmp/.hardwareinfo.$$
   
 #list detected hard disks  
   echo "Detected Hard Disks:" >> /tmp/.hardwareinfo.$$
   $PCSYS disk-list >> /tmp/.hardwareinfo.$$
   echo " " >> /tmp/.hardwareinfo.$$
 
 #detect an active network card.  Also now lists hard disk info and checks for a sound card.
   ifconfig | grep -q 'UP'
   if [ $? -eq 0 ] ; then
     echo "Compatible Network Interface Detected:" >> /tmp/.hardwareinfo.$$
     ifconfig -l >> /tmp/.hardwareinfo.$$
     echo " " >> /tmp/.hardwareinfo.$$
     sed  -i '' 's/lo0//g' /tmp/.hardwareinfo.$$
     sed  -i '' 's/fwe//g' /tmp/.hardwareinfo.$$
     sed  -i '' 's/ipfw//g' /tmp/.hardwareinfo.$$
     sed  -i '' 's/pfsync//g' /tmp/.hardwareinfo.$$
     sed  -i '' 's/pflog//g' /tmp/.hardwareinfo.$$
     sed  -i '' 's/usbus//g' /tmp/.hardwareinfo.$$
     sed  -i '' 's/tun//g' /tmp/.hardwareinfo.$$
   else
     echo "No Compatible Network Interface Detected:" >> /tmp/.hardwareinfo.$$  
     echo " " >> /tmp/.hardwareinfo.$$
     fi
   
 #check active sound devices
   cat /dev/sndstat | grep 'pcm0:'
      if [ $? -eq 0 ] ; then
	echo "Compatible Sound Device(s) Detected:" >> /tmp/.hardwareinfo.$$
	cat /dev/sndstat >> /tmp/.hardwareinfo.$$
	echo " " >> /tmp/.hardwareinfo.$$
      else
	echo "No compatible Sound Device(s) Detected."  >> /tmp/.hardwareinfo.$$
	echo " " >> /tmp/.hardwareinfo.$$
      fi
   sed -i '' '/Installed devices:/d' /tmp/.hardwareinfo.$$
   dialog --tailbox /tmp/.hardwareinfo.$$ 30 60
   rm /tmp/.hardwareinfo.$$
   
}


get_target_part()
{
  # Now prompt for the full-disk or partition to install onto
  ${PCSYS} disk-part $SYSDISK > /tmp/.dList.$$
  dOpts="ALL \"Use entire disk\" on"
  dFmt=`grep "$SYSDISK-format:" /tmp/.dList.$$ | awk '{print $2}'` 
  if [ "$dFmt" = "MBR" ] ; then
    dChar="s"
    DISKFORMAT="MBR"
  else
    dChar="p"
    DISKFORMAT="GPT"
  fi
  i=1
  while :
  do
     partRAW="${dChar}${i}"
     part="${SYSDISK}${dChar}${i}"
     [ -e "/dev/${part}" ] || break
     desc="`cat /tmp/.dList.$$ | grep ^${part}-label | cut -d ':' -f 2`"
     mb="`cat /tmp/.dList.$$ | grep ^${part}-sizemb | awk '{print $2}'`"
     dOpts="$dOpts $partRAW \"${mb}MB -$desc\" off" 
     i="`expr $i + 1`"
  done
  rm /tmp/.dList.$$
  get_dlg_ans "--radiolist \"Select target partition\" 12 80 5 ${dOpts}"
  if [ -z "$ANS" ] ; then
     exit_err "Invalid disk selected!"
  fi
  DISKPART="$ANS"

  #Add a while loop that will prompt for the disk format on a full disk install
  if [ "$DISKPART" = "ALL" ] ; then
     while :
     do
	get_dlg_ans "--menu \"Select the disk format you would like to use.\" 12 45 10 1. GPT 2. MBR"
	if [ -z "$ANS" ] ; then
	  echo "Invalid disk format entered!" 
	  continue
	else
	  break
	fi
    done
    if [ "1." = "$ANS" ] ; then
      DISKFORMAT="GPT"
    else 
      DISKFORMAT="MBR"
    fi
  fi
}

get_root_pw()
{

# [a-z]*[A-Z]*[0-9]*[!\"$%^&*()_+=#'`@~:?<>|{}\\x5b;\\-.,\\x5d]*)+
  while :
  do
    get_dlg_ans "--passwordbox 'Enter the root password' 8 30"
    if [ -z "$ANS" ] ; then
       echo "Invalid password entered!  Please Enter a valid Password!" >> /tmp/.vartemp.$$
       dialog --tailbox /tmp/.vartemp.$$ 8 67
       rm /tmp/.vartemp.$$
       continue
    fi
    #   Check for invalid characters
    echo "$ANS" | grep -q '^[a-zA-Z0-9`~!@#$%^&*-_+=|\:;<,>.?/~`''""(()){{}}-]*$'
    if [ $? -eq 0 ] ; then     
    else   
       echo "Password contains invalid characters!" >> /tmp/.vartemp.$$
       dialog --tailbox /tmp/.vartemp.$$ 8 40
       rm /tmp/.vartemp.$$
       continue      
    fi
    ROOTPW="$ANS"
    get_dlg_ans "--passwordbox 'Confirm root password' 8 30"
    if [ -z "$ANS" ] ; then
       echo "Invalid password entered!  Please Enter a Password!" >> /tmp/.vartemp.$$
       dialog --tailbox /tmp/.vartemp.$$ 8 67
       rm /tmp/.vartemp.$$
       continue
    fi
    ROOTPWCONFIRM="$ANS"
    if [ "$ROOTPWCONFIRM" = "$ROOTPW" ] ; then break; fi
    dialog --title "$TITLE" --yesno 'Password Mismatch, try again?' 8 40
    if [ $? -eq 0 ] ; then continue ; fi
    exit_err "Failed setting root password!"
  done
}

get_user_pw()

{
  while :
  do
    get_dlg_ans "--passwordbox \"Enter the password for $USERNAME\" 8 40"
    if [ -z "$ANS" ] ; then
       echo "Invalid password entered!  Please Enter a Password!" >> /tmp/.vartemp.$$
       dialog --tailbox /tmp/.vartemp.$$ 8 35
       rm /tmp/.vartemp.$$
       continue
    fi
    # Check for invalid characters
    echo "$ANS" | grep -q '^[a-zA-Z0-9`~!@#$%^&*-_+=|\:;<,>.?/~`''""(()){{}}-]*$'
    if [ $? -eq 0 ] ; then      
    else   
       echo "Password contains invalid characters!" >> /tmp/.vartemp.$$
       dialog --tailbox /tmp/.vartemp.$$ 8 40
       rm /tmp/.vartemp.$$
       continue  
    fi
    USERPW="$ANS"   
    get_dlg_ans "--passwordbox 'Confirm password' 8 40"
    if [ -z "$ANS" ] ; then
       echo "Invalid password entered!  Please Enter a Password!" >> /tmp/.vartemp.$$
       dialog --tailbox /tmp/.vartemp.$$ 8 35
       rm /tmp/.vartemp.$$
       continue
    fi
    USERPWCONFIRM="$ANS"
    if [ "$USERPWCONFIRM" = "$USERPW" ] ; then break; fi
    dialog --title "$TITLE" --yesno 'Password Mismatch, try again?' 8 30
    if [ $? -eq 0 ] ; then continue ; fi
    exit_err "Failed setting password!"
  done
  
}

get_user_name()
{
  while :
  do
    #Ask for user name and make sure it is not empty
    get_dlg_ans "--inputbox 'Enter a username' 8 40"
    if [ -z "$ANS" ] ; then
       echo "Invalid username entered!" >> /tmp/.vartemp.$$
       dialog --tailbox /tmp/.vartemp.$$ 8 35
       rm /tmp/.vartemp.$$
       continue
    fi   
    #check for invalid characters
    echo "$ANS" | grep -q '^[a-zA-Z0-9]*$'
    if [ $? -eq 1 ] ; then
       echo "Name contains invalid characters!" >> /tmp/.vartemp.$$
       dialog --tailbox /tmp/.vartemp.$$ 8 35
       rm /tmp/.vartemp.$$
       continue      
    fi
    USERNAME="$ANS"
    break
  done
  
}

get_user_realname()
{
  while :
  do
    #ask for user's real name
    get_dlg_ans "--inputbox \"Enter the real name for $USERNAME\" 8 40"
    if [ -z "$ANS" ] ; then
       echo "Real name can not be blank"  >> /tmp/.vartemp.$$
       dialog --tailbox /tmp/.vartemp.$$ 8 35
       rm /tmp/.vartemp.$$
       continue
    fi
    #check for invalid characters
    echo "$ANS" | grep -q '^[a-zA-Z ]*$'
    if [ $? -eq 1 ] ; then
       echo "Name contains invalid characters!" >> /tmp/.vartemp.$$
       dialog --tailbox /tmp/.vartemp.$$ 8 35
       rm /tmp/.vartemp.$$
       continue  
    else 
       break
    fi
  done  
  USERREALNAME="$ANS"
}

get_user_shell()
{
  while :
  do
    get_dlg_ans "--menu \"Select the users shell\" 12 45 10 /bin/sh SH /bin/csh CSH /bin/tcsh TCSH /usr/local/bin/bash BASH"
    if [ -z "$ANS" ] ; then
      echo "Invalid SHELL entered!" 
      continue
    else
      break
    fi
  done
  USERSHELL="$ANS"
}

get_hostname()
{
  while :
  do
    get_dlg_ans "--inputbox \"Enter a system Hostname\" 8 35"
      if [ -z "$ANS" ] ; then
      echo "Hostname can not be blank"  >> /tmp/.vartemp.$$
      dialog --tailbox /tmp/.vartemp.$$ 8 30
      rm /tmp/.vartemp.$$
      continue
      fi
    echo "$ANS" | grep -q '^[a-zA-Z0-9.-]*$'
    if [ $? -eq 1 ] ; then
      echo "Hostname contains invalid characters!" >> /tmp/.vartemp.$$
      dialog --tailbox /tmp/.vartemp.$$ 8 48
      rm /tmp/.vartemp.$$
      continue  
    else 
      break
    fi
  done
  SYSHOSTNAME="$ANS"
}

get_sshd()
{
  SYSSSHD=""
  dialog --title "$TITLE" --yesno 'Enable SSH?' 8 30
  if [ $? -ne 0 ] ; then return ; fi
  SYSSSHD="YES"
}

get_netconfig()
{
  SYSNIC=""
  SYSNICIP=""
  SYSNICDNS=""
  SYSNICMASK=""
  SYSNICROUTE=""

  dialog --title "$TITLE" --yesno 'Enable networking?' 6 30
  if [ $? -ne 0 ] ; then return ; fi

  dOpts="auto \"Automatic DHCP\" on"
  ${PCSYS} detect-nics > /tmp/.dList.$$
  while read i
  do
     d="`echo $i | cut -d ':' -f 1`"
     desc="`echo $i | cut -d '<' -f 2 | cut -d '>' -f 1`"
     dOpts="$dOpts $d \"$desc\" off"
  done < /tmp/.dList.$$
  rm /tmp/.dList.$$
  get_dlg_ans "--radiolist \"Select network card to configure\" 12 50 5 ${dOpts}"
  if [ -z "$ANS" ] ; then
     exit_err "Invalid NIC selected!"
  fi
  SYSNIC="$ANS"
  if [ "$SYSNIC" = "auto" ] ; then
     SYSNIC="AUTO-DHCP-SLAAC"
     return
  fi

  #Set IP Address and check for invalid characters
  while :
  do
    get_dlg_ans "--inputbox \"Enter the IP address for $SYSNIC\" 8 40"
      if [ -z "$ANS" ] ; then
      echo "IP can not be blank"  >> /tmp/.vartemp.$$
      dialog --tailbox /tmp/.vartemp.$$ 8 30
      rm /tmp/.vartemp.$$
      continue
      fi
    echo "$ANS" | grep -q '^[0-9.]*$'
    if [ $? -eq 1 ] ; then
      echo "IP contains invalid characters!" >> /tmp/.vartemp.$$
      dialog --tailbox /tmp/.vartemp.$$ 8 38
      rm /tmp/.vartemp.$$
      continue  
    else 
      break
    fi
  done
  SYSNICIP="$ANS"

  #Set Netmask and check for invalid characters
  while :
  do
    get_dlg_ans "--inputbox \"Enter the netmask for $SYSNIC\" 8 40"
      if [ -z "$ANS" ] ; then
      echo "Netmask can not be blank"  >> /tmp/.vartemp.$$
      dialog --tailbox /tmp/.vartemp.$$ 8 35
      rm /tmp/.vartemp.$$
      continue
    fi
    echo "$ANS" | grep -q '^[0-9.]*$'
    if [ $? -eq 1 ] ; then
      echo "Netmask contains invalid characters!" >> /tmp/.vartemp.$$
      dialog --tailbox /tmp/.vartemp.$$ 8 45
      rm /tmp/.vartemp.$$
      continue  
    else 
      break
    fi
  done   
  SYSNICMASK="$ANS"

  #Set DNS and check for invalid characters
  while :
  do
    get_dlg_ans "--inputbox \"Enter the DNS address for $SYSNIC\" 8 40"
      if [ -z "$ANS" ] ; then
      echo "DNS can not be blank!"  >> /tmp/.vartemp.$$
      dialog --tailbox /tmp/.vartemp.$$ 8 35
      rm /tmp/.vartemp.$$
      continue
      fi
    echo "$ANS" | grep -q '^[0-9.]*$'
    if [ $? -eq 1 ] ; then
      echo "DNS contains invalid characters!" >> /tmp/.vartemp.$$
      dialog --tailbox /tmp/.vartemp.$$ 8 45
      rm /tmp/.vartemp.$$
      continue  
    else 
      break
    fi
  done
  SYSNICDNS="$ANS"

  #Set Gateway and check for invalid characters
  while :
  do
    get_dlg_ans "--inputbox \"Enter the Gateway address for $SYSNIC\" 8 40"
      if [ -z "$ANS" ] ; then
      echo "Gateway can not be blank!"  >> /tmp/.vartemp.$$
      dialog --tailbox /tmp/.vartemp.$$ 8 35
      rm /tmp/.vartemp.$$
      continue
      fi
    echo "$ANS" | grep -q '^[0-9.]*$'
    if [ $? -eq 1 ] ; then
      echo "Gateway contains invalid characters!" >> /tmp/.vartemp.$$
      dialog --tailbox /tmp/.vartemp.$$ 8 45
      rm /tmp/.vartemp.$$
      continue  
    else 
      break
    fi
  done
  SYSNICROUTE="$ANS"

}

gen_pc-sysinstall_cfg()
{
   # Start the header information
   echo "# Auto-Generated pc-sysinstall configuration" >${CFGFILE}
   echo "installInteractive=no" >>${CFGFILE}
   echo "installMode=fresh" >>${CFGFILE}
   if [ "$SYSTYPE" = "desktop" ] ; then
     echo "installType=PCBSD" >>${CFGFILE}
   else
     echo "installType=FreeBSD" >>${CFGFILE}
   fi
   echo "packageType=dist" >> ${CFGFILE}
   if [ "`uname -m`" = "amd64" ] ; then
     echo "distFiles=base doc games kernel lib32" >> ${CFGFILE}
   else
     echo "distFiles=base doc games kernel" >> ${CFGFILE}
   fi
   echo "installMedium=local" >>${CFGFILE}
   echo "localPath=/dist" >>${CFGFILE}

   if [ -n "$SYSHOSTNAME" ] ; then
      echo "" >> ${CFGFILE}
      echo "hostname=$SYSHOSTNAME" >> ${CFGFILE}
   fi
   if [ -n "$SYSNIC" ] ; then
      echo "" >> ${CFGFILE}
      echo "netSaveDev=$SYSNIC" >> ${CFGFILE}
      if [ "$SYSNIC" != "AUTO-DHCP-SLAAC" ] ; then
        echo "netSaveIP_${SYSNIC}=$SYSNICIP" >> ${CFGFILE}
        echo "netSaveMask_${SYSNIC}=$SYSNICMASK" >> ${CFGFILE}
        echo "netSaveNameServer_${SYSNIC}=$SYSNICDNS" >> ${CFGFILE}
        echo "netSaveDefaultRouter_${SYSNIC}=$SYSNICROUTE" >> ${CFGFILE}
      fi
   fi


   # Now do the disk block
   echo "" >> ${CFGFILE}
   echo "# Disk Setup for $SYSDISK" >> ${CFGFILE}
   echo "disk0=$SYSDISK" >> ${CFGFILE}
   echo "partition=$DISKPART" >> ${CFGFILE}
   echo "bootManager=${SYSBOOTMANAGER}" >> ${CFGFILE}
   echo "partscheme=$DISKFORMAT" >> ${CFGFILE}
   echo "commitDiskPart" >> ${CFGFILE}

   # Now the partition block
   echo "" >> ${CFGFILE}
   echo "# Partition Setup for ${SYSDISK}($DISKPART)" >> ${CFGFILE}
   echo "# All sizes are expressed in MB" >> ${CFGFILE}
   echo "# Avail FS Types, UFS, UFS+S, UFS+SUJ, UFS+J, ZFS, SWAP" >> ${CFGFILE}
   echo "# UFS.eli, UFS+S.eli, UFS+SUJ, UFS+J.eli, ZFS.eli, SWAP.eli" >> ${CFGFILE}

   # What file-system are we using now?
   FSTAG="ZFS"
   if [ "$USINGGELI" = "YES" ] ; then FSTAG="ZFS.eli"; fi

   # Doing a single disk zpool, or a mirror/raidz[1-3]?
   if [ "$ZPOOL_TYPE" = "single" ] ; then
     echo "disk0-part=$FSTAG 0 ${ZFSLAYOUT}" >> ${CFGFILE}
   else
     echo "disk0-part=$FSTAG 0 ${ZFSLAYOUT} (${ZPOOL_TYPE}: `echo $ZPOOL_DISKS | sed 's| |,|g'`)" >> ${CFGFILE}
   fi

   # If using GELI encryption, add it to config file
   if [ "$USINGGELI" = "YES" ] ; then
     echo "encpass=$GELIPASS" >> ${CFGFILE}
   fi

   echo "disk0-part=SWAP.eli 2000 none" >> ${CFGFILE}
   echo "commitDiskLabel" >> ${CFGFILE}
   echo "" >> ${CFGFILE}

   # Now the packages
   if [ "$SYSTYPE" = "desktop" ] ; then
     echo "installPackages=misc/pcbsd-base misc/pcbsd-meta-kde ${EXTRAPKGS}" >> ${CFGFILE}
     echo "" >> ${CFGFILE}
     # Set our markers for desktop to run the first-time boot wizards
     echo "runCommand=touch /var/.runxsetup" >> ${CFGFILE}
     echo "runCommand=touch /var/.pcbsd-firstboot" >> ${CFGFILE}
     echo "runCommand=touch /var/.pcbsd-firstgui" >> ${CFGFILE}
   else
     echo "installPackages=misc/trueos-base sysutils/pcbsd-appweb ${EXTRAPKGS}" >> ${CFGFILE}
     echo "" >> ${CFGFILE}
     echo "" >> ${CFGFILE}

     # Since on TrueOS, lets save username / passwords
     echo "rootPass=${ROOTPW}" >> ${CFGFILE}
     echo "" >> ${CFGFILE}
     echo "userName=${USERNAME}" >> ${CFGFILE}
     echo "userPass=${USERPW}" >> ${CFGFILE}
     echo "userShell=${USERSHELL}" >> ${CFGFILE}
     echo "userHome=/home/${USERNAME}" >> ${CFGFILE}
     echo "userGroups=wheel,operator" >> ${CFGFILE}
     echo "commitUser" >> ${CFGFILE}
   fi

   # If AppCafe is enabled
   if [ -n "$APPUSER" ] ; then
     # Save appcafe data to file
     echo "$APPUSER" > /tmp/appcafe-user
     echo "$APPPASS" > /tmp/appcafe-pass
     echo "$APPPORT" > /tmp/appcafe-port

     # Now add pc-sysinstall config stuff
     echo "" >> ${CFGFILE}
     echo 'runExtCommand=mv /tmp/appcafe-user ${FSMNT}/tmp/' >> ${CFGFILE}
     echo 'runExtCommand=mv /tmp/appcafe-pass ${FSMNT}/tmp/' >> ${CFGFILE}
     echo 'runExtCommand=mv /tmp/appcafe-port ${FSMNT}/tmp/' >> ${CFGFILE}
   fi

   # Run the sys-init
   if [ "$SYSTYPE" = "desktop" ] ; then
     echo "runCommand=sh /usr/local/share/pcbsd/scripts/sys-init.sh desktop en_US" >> ${CFGFILE}
   else
     echo "runCommand=sh /usr/local/share/pcbsd/scripts/sys-init.sh server" >> ${CFGFILE}
   fi

   # Now add the freebsd dist files so warden can create a template on first boot
   echo 'runCommand=mkdir -p /usr/local/tmp/warden-dist/' >> ${CFGFILE}
   echo 'runExtCommand=cp /dist/*.txz ${FSMNT}/usr/local/tmp/warden-dist/' >> ${CFGFILE}

   # Last cleanup stuff
   echo "" >> ${CFGFILE}
   echo "runExtCommand=/root/save-config.sh" >> ${CFGFILE}
   echo "runCommand=newaliases" >> ${CFGFILE}

   # Are we enabling SSHD?
   if [ "$SYSSSHD" = "YES" ] ; then
     echo "runCommand=echo 'sshd_enable=\"YES\"' >> /etc/rc.conf" >> ${CFGFILE}
   fi
}
   


#ask if user wants to install appweb
zans_appweb()
{
   if dialog --yesno "Do you want to enable remote access to the AppCafe browser based package manager now?  You will be asked to setup an additional user name and password"  8 60; then
     install_appweb
   fi
}

#ask for AppWeb User Name
appweb_user() {
  while :
  do
    #Ask for user name and make sure it is not empty
    get_dlg_ans "--inputbox 'Enter a username for AppWeb' 8 40"
    if [ -z "$ANS" ] ; then
       echo "Invalid username entered!" >> /tmp/.vartemp.$$
       dialog --tailbox /tmp/.vartemp.$$ 8 35
       rm /tmp/.vartemp.$$
       continue
    fi   
    #check for invalid characters
    echo "$ANS" | grep -q '^[a-zA-Z0-9]*$'
    if [ $? -eq 1 ] ; then
       echo "Name contains invalid characters!" >> /tmp/.vartemp.$$
       dialog --tailbox /tmp/.vartemp.$$ 8 35
       rm /tmp/.vartemp.$$
       continue      
    fi
    APPUSER="$ANS"
    break
  done
}

#ask for AppWeb Password
appweb_pass() {
  while :
  do
    get_dlg_ans "--passwordbox \"Enter the password for $APPUSER\" 8 40"
    if [ -z "$ANS" ] ; then
       echo "Invalid password entered!  Please Enter a Password!" >> /tmp/.vartemp.$$
       dialog --tailbox /tmp/.vartemp.$$ 8 35
       rm /tmp/.vartemp.$$
       continue
    fi
    # Check for invalid characters
    echo "$ANS" | grep -q '^[a-zA-Z0-9`~!@#$%^&*-_+=|\:;<,>.?/~`''""(()){{}}-]*$'
    if [ $? -eq 0 ] ; then      
    else   
       echo "Password contains invalid characters!" >> /tmp/.vartemp.$$
       dialog --tailbox /tmp/.vartemp.$$ 8 40
       rm /tmp/.vartemp.$$
       continue  
    fi
    APPPASS="$ANS"   
    get_dlg_ans "--passwordbox 'Confirm password' 8 40"
    if [ -z "$ANS" ] ; then
       echo "Invalid password entered!  Please Enter a Password!" >> /tmp/.vartemp.$$
       dialog --tailbox /tmp/.vartemp.$$ 8 35
       rm /tmp/.vartemp.$$
       continue
    fi
    APPPWCONFIRM="$ANS"
    if [ "$APPPWCONFIRM" = "$APPPASS" ] ; then break; fi
    dialog --title "$TITLE" --yesno 'Password Mismatch, try again?' 8 30
    if [ $? -eq 0 ] ; then continue ; fi
    exit_err "Failed setting password!"
  done
}

appweb_port()
{
  while :
  do
    get_dlg_ans "--inputbox \"Enter the port to listen on.  The default is 8885.\" 8 35"
    if [ -z "$ANS" ] ; then
      echo "Port number can not be blank"  >> /tmp/.vartemp.$$
      dialog --tailbox /tmp/.vartemp.$$ 8 40
      rm /tmp/.vartemp.$$
      continue
    fi
    echo "$ANS" | grep -q '^[0-9]*$'
    if [ $? -eq 1 ] ; then
      echo "Port number contains invalid characters!" >> /tmp/.vartemp.$$
      dialog --tailbox /tmp/.vartemp.$$ 8 48
      rm /tmp/.vartemp.$$
      continue  
    else 
      break
    fi
  done
  APPPORT="$ANS"
}

change_packages()
{
  #ask which  meta package the user would like to install
  get_dlg_ans "--checklist \"Meta Packages\" 12 50 5 Devel \"Devel Role\" off FreeNAS \"FreeNas Role\" off Office \"Office Role\" off Server \"Server Role\" off"
  if [ -z "$ANS" ] ; then
    exit_err "Invalid meta package type"
  fi
  
  for i in $ANS
  do
    if [ "$i" = "Devel" ] ; then 
	EXTRAPKGS="$EXTRAPKGS misc/pcbsd-role-devel"
    fi
    if [ "$i" = "FreeNAS" ] ; then  
	EXTRAPKGS="$EXTRAPKGS misc/pcbsd-role-freenasbuild"
    fi
    if [ "$i" = "Office" ] ; then 
	EXTRAPKGS="$EXTRAPKGS misc/pcbsd-role-office"
    fi
    if [ "$i" = "Server" ] ; then 
	EXTRAPKGS="$EXTRAPKGS misc/pcbsd-role-server"
    fi
  done

  #remake the install script 
  gen_pc-sysinstall_cfg
}

change_disk_selection() {
  get_target_disk
  get_target_part
  get_sys_bootmanager
  gen_pc-sysinstall_cfg
}

start_full_wizard() 
{
  # Start the wizard
  get_sys_type
  get_target_disk
  get_target_part
  get_sys_bootmanager

  # If doing a server setup, need to prompt for some more details
  if [ "$SYSTYPE" = "server" ] ; then
     get_root_pw
     get_user_name
     get_user_pw
     get_user_realname
     get_user_shell
     change_networking
  fi
  zans_appweb
  gen_pc-sysinstall_cfg
}

# Adjust network options
change_networking() {
  get_hostname
  get_netconfig
  get_sshd
  gen_pc-sysinstall_cfg
}

# Setup appweb and syscache
install_appweb() {
  appweb_user
  appweb_pass
  appweb_port
}

start_edit_menu_loop()
{

  while :
  do
    dialog --title "PC-BSD Text Install - Edit Menu" --menu "Please select from the following options:" 18 40 10 disk "Change disk ($SYSDISK)" zpool "Change zpool settings" zfs "Change ZFS layout" network "Change networking" packages "choose meta-packages to install" view "View install script" edit "Edit install script" back "Back to main menu" 2>/tmp/answer
    if [ $? -ne 0 ] ; then break ; fi

    ANS="`cat /tmp/answer`"

    case $ANS in
       disk) change_disk_selection
             ;;
    network) change_networking 
	     ;;
        zfs) change_zfs
	     ;;
      zpool) change_zpool
	     ;;
   packages) change_packages
	     ;;
       view) more ${CFGFILE}
             rtn
             ;;
       edit) ee ${CFGFILE}
             rtn
             ;;
       back) break ;;
          *) ;;
    esac
  done

}

start_menu_loop()
{

  while :
  do
    dialog --title "PC-BSD Text Install" --menu "Please select from the following options:" 18 40 10 install "Start the installation" wizard "Re-run install wizard" edit "Edit install options" hardware "check compatibility" quit "Quit install wizard" 2>/tmp/answer
    if [ $? -ne 0 ] ; then break ; fi

    ANS="`cat /tmp/answer`"

    case $ANS in
     wizard) start_full_wizard
             ;;
       edit) start_edit_menu_loop
             ;;
    install) dialog --title "$TITLE" --yesno 'This will begin the installation, continue?' 8 30
             if [ $? -eq 0 ] ; then
                ${PCSYS} -c ${CFGFILE}
                rtn
             fi
             ;;
   hardware) get_hardware_info
	     ;;
       quit) break ;;
          *) ;;
    esac
  done

}



if [ -e "$CFGFILE" ] ; then
   cp ${CFGFILE} ${CFGFILE}.bak
   rm ${CFGFILE}
fi

start_full_wizard
start_menu_loop

exit 0
