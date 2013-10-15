#!/bin/sh
# License: BSD
# Author: Kris Moore (kris@pcbsd.org)
########################################################
# This script is fairly linear, it will walk through a series of questions
# and when finished, generate a pc-sysinstall script

# Source our functions
. /usr/local/share/pcbsd/scripts/functions.sh

TITLE="PC-BSD Install Dialog"

TANS="/tmp/.pcinsdialog.$$"

CFGFILE="/tmp/sys-install.cfg"

get_dlg_ans()
{
    if [ -e "$TANS" ] ; then rm ${TANS}; fi
    if [ -e "$TANS.dlg" ] ; then rm ${TANS}.dlg; fi
    echo "dialog --title \"$TITLE\" ${@}" >${TANS}.dlg
    sh ${TANS}.dlg 2>${TANS}
    exit=$?
    if [ ! -e "$TANS" ] ; then
       ANS=""
       return $exit
    fi
    ANS=`cat ${TANS}`
    return $exit
}


get_sys_type()
{
  # First ask the system type
  get_dlg_ans "--radiolist \"System type\" 12 50 5 desktop \"PC-BSD Desktop\" on server \"TrueOS / FreeBSD Server\" off"
  if [ -z "$ANS" ] ; then
     exit_err "Invalid system type"
  fi
  SYSTYPE="$ANS"
}


get_target_disk()
{
  # Now we prompt for the disk to install on
  pc-sysinstall disk-list > /tmp/.dList.$$
  while read i
  do
     fOpt="on"
     d=`echo $i | cut -d ':' -f 1`
     desc=`echo $i | cut -d ':' -f 2`
     dOpts="$dOpts $d \"$desc\" $fOpt" 
     if [ -z "$fOpt" ] ; then fOpt="off"; fi
  done < /tmp/.dList.$$
  rm /tmp/.dList.$$
  get_dlg_ans "--radiolist \"Select target disk\" 12 50 5 ${dOpts}"
  if [ -z "$ANS" ] ; then
     exit_err "Invalid disk selected!"
  fi
  SYSDISK="$ANS"
}

get_target_part()
{
  # Now prompt for the full-disk or partition to install onto
  pc-sysinstall disk-part $SYSDISK > /tmp/.dList.$$
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
}

get_root_pw()
{
  while :
  do
    get_dlg_ans "--passwordbox 'Enter the root password' 8 30"
    if [ -z "$ANS" ] ; then
       exit_err "Invalid password entered!"
    fi
    ROOTPW="$ANS"
    get_dlg_ans "--passwordbox 'Confirm root password' 8 30"
    if [ -z "$ANS" ] ; then
       exit_err "Invalid password entered!"
    fi
    ROOTPWCONFIRM="$ANS"
    if [ "$ROOTPWCONFIRM" = "$ROOTPW" ] ; then break; fi
    get_dlg_ans "--yesno 'Password Mismatch, try again?' 8 30"
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
       exit_err "Invalid password entered!"
    fi
    USERPW="$ANS"
    get_dlg_ans "--passwordbox 'Confirm password' 8 40"
    if [ -z "$ANS" ] ; then
       exit_err "Invalid password entered!"
    fi
    USERPWCONFIRM="$ANS"
    if [ "$USERPWCONFIRM" = "$USERPW" ] ; then break; fi
    get_dlg_ans "--yesno 'Password Mismatch, try again?' 8 30"
    if [ $? -eq 0 ] ; then continue ; fi
    exit_err "Failed setting password!"
  done
}

get_user_name()
{
    get_dlg_ans "--inputbox 'Enter a username' 8 40"
    if [ -z "$ANS" ] ; then
       exit_err "Invalid username entered!"
    fi
    USERNAME="$ANS"
}

get_user_realname()
{
    get_dlg_ans "--inputbox \"Enter the real name for $USERNAME\" 8 40"
    if [ -z "$ANS" ] ; then
       exit_err "Invalid real name entered!"
    fi
    USERREALNAME="$ANS"
}

get_user_shell()
{
    get_dlg_ans "--menu \"Select the users shell\" 12 45 10 /bin/sh SH /bin/csh CSH /bin/tcsh TCSH /bin/bash BASH"
    if [ -z "$ANS" ] ; then
       exit_err "Invalid SHELL entered!"
    fi
    USERSHELL="$ANS"
}

gen_pc-sysinstall_cfg()
{
   # Start the header information
   echo "# Auto-Generated pc-sysinstall configuration" >${CFGFILE}
   echo "installInteractive=no" >>${CFGFILE}
   echo "installMode=fresh" >>${CFGFILE}
   if [ "$SYSTYPE" = "desktop" ] ; then
     echo "installType=PCBSD" >>${CFGFILE}
     echo "netSaveDev=AUTO-DHCP-SLAAC" >> ${CFGFILE}
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

   # Now do the disk block
   echo "" >> ${CFGFILE}
   echo "# Disk Setup for $SYSDISK" >> ${CFGFILE}
   echo "disk0=$SYSDISK" >> ${CFGFILE}
   echo "partition=$DISKPART" >> ${CFGFILE}
   echo "bootManager=GRUB" >> ${CFGFILE}
   echo "partscheme=$DISKFORMAT" >> ${CFGFILE}
   echo "commitDiskPart" >> ${CFGFILE}

   # Now the partition block
   echo "" >> ${CFGFILE}
   echo "# Partition Setup for ${SYSDISK}($DISKPART)" >> ${CFGFILE}
   echo "# All sizes are expressed in MB" >> ${CFGFILE}
   echo "# Avail FS Types, UFS, UFS+S, UFS+SUJ, UFS+J, ZFS, SWAP" >> ${CFGFILE}
   echo "# UFS.eli, UFS+S.eli, UFS+SUJ, UFS+J.eli, ZFS.eli, SWAP.eli" >> ${CFGFILE}
   echo "disk0-part=ZFS 0 /,/root,/tmp(compress=lz4),/usr(canmount=off),/usr/home,/usr/jails,/usr/obj(compress=lz4),/usr/pbi,/usr/ports(compress=lz4),/usr/ports/distfiles(compress=off),/usr/src(compress=lz4),/var(canmount=off),/var/audit(compress=lz4),/var/log(compress=lz4),/var/tmp(compress=lz4)" >> ${CFGFILE}
   echo "disk0-part=SWAP 2000 none" >> ${CFGFILE}
   echo "commitDiskLabel" >> ${CFGFILE}
   echo "" >> ${CFGFILE}

   # Now the packages
   if [ "$SYSTYPE" = "desktop" ] ; then
     echo "installPackages=pcbsd-base pcbsd-meta-kde pcbsd-meta-kde-accessibility pcbsd-meta-kde-artwork pcbsd-meta-kde-graphics pcbsd-meta-kde-multimedia pcbsd-meta-kde-network pcbsd-meta-kde-pim" >> ${CFGFILE}
     echo "" >> ${CFGFILE}
     # Set our markers for desktop to run the first-time boot wizards
     echo "runCommand=sh /usr/local/share/pcbsd/scripts/sys-init.sh desktop en_US" >> ${CFGFILE}
     echo "runCommand=touch /var/.runxsetup" >> ${CFGFILE}
     echo "runCommand=touch /var/.pcbsd-firstboot" >> ${CFGFILE}
     echo "runCommand=touch /var/.pcbsd-firstgui" >> ${CFGFILE}
   else
     echo "installPackages=trueos-base" >> ${CFGFILE}
     echo "" >> ${CFGFILE}
     echo "runCommand=sh /usr/local/share/pcbsd/scripts/sys-init.sh server" >> ${CFGFILE}
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

     # KPM - TODO, add blocks to create network config, hostname / ssh settings
   fi

   # Last cleanup stuff
   echo "" >> ${CFGFILE}
   echo "runExtCmd=/root/save-config.sh" >> ${CFGFILE}
   echo "runCommand=newaliases" >> ${CFGFILE}
  
 
}

# Start the wizard
get_sys_type
get_target_disk
get_target_part

# If doing a server setup, need to prompt for some more details
if [ "$SYSTYPE" = "server" ] ; then
   get_root_pw
   get_user_name
   get_user_pw
   get_user_realname
   get_user_shell
fi

gen_pc-sysinstall_cfg
exit 0
