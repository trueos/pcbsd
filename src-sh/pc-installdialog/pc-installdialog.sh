#!/bin/sh
# License: BSD
# Author: Kris Moore (kris@pcbsd.org)
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
ZFSLAYOUT="/,/root,/tmp(compress=lz4),/usr(canmount=off),/usr/home,/usr/jails,/usr/obj(compress=lz4),/usr/pbi,/usr/ports(compress=lz4),/usr/ports/distfiles(compress=off),/usr/src(compress=lz4),/var(canmount=off),/var/audit(compress=lz4),/var/log(compress=lz4),/var/tmp(compress=lz4)"

# Ugly master list of settable ZFS properties
ZPROPS="aclinherit(discard|noallow|restricted|passthrough|passthrough-x),aclmode(discard|groupmask|passthrough|restricted),atime(on|off),canmount(on|off|noauto),checksum(on|off|fletcher2|fletcher4|sha256),compress(on|off|lzjb|gzip|zle|lz4),copies(1|2|3),dedup(on|off|verify|sha256),exec(on|off),primarycache(all|none|metadata),readonly(on|off),secondarycache(all|none|metadata),setuid(on|off),sharenfs(on|off),logbias(latency|throughput),snapdir(hidden|visible),sync(standard|always|disabled),jailed(off|on)"

PCSYS="/usr/local/sbin/pc-sysinstall"

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
  # First ask the system type
  get_dlg_ans "--radiolist \"System type\" 12 50 5 desktop \"PC-BSD Desktop\" on server \"TrueOS / FreeBSD Server\" off"
  if [ -z "$ANS" ] ; then
     exit_err "Invalid system type"
  fi
  SYSTYPE="$ANS"

  # If on a desktop, lets just set DHCP
  if [ "$SYSTYPE" = "desktop" ] ; then
     SYSNIC="AUTO-DHCP-SLAAC"
  fi
}

get_sys_bootmanager()
{
  # Ask the boot-manager
  get_dlg_ans "--radiolist \"Boot Manager\" 12 50 5 GRUB \"GRUB - Recommended\" on BSD \"FreeBSD Boot-Loader\" off none \"No boot-loader\" off"
  if [ -z "$ANS" ] ; then
     exit_err "Invalid bootmanager type"
  fi
  SYSBOOTMANAGER="$ANS"
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
    dialog --title "$TITLE" --yesno 'Password Mismatch, try again?' 8 30
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
    dialog --title "$TITLE" --yesno 'Password Mismatch, try again?' 8 30
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
	ANS="$USERNAME"
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

get_hostname()
{
    get_dlg_ans "--inputbox 'Enter a system Hostname' 8 40"
    if [ -z "$ANS" ] ; then
       exit_err "Invalid hostname entered!"
    fi
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

  get_dlg_ans "--inputbox \"Enter the IP address for $SYSNIC\" 8 40"
  if [ -z "$ANS" ] ; then
     exit_err "Invalid IP entered!"
  fi
  SYSNICIP="$ANS"

  get_dlg_ans "--inputbox \"Enter the Netmask for $SYSNIC\" 8 40"
  if [ -z "$ANS" ] ; then
     exit_err "Invalid real name entered!"
  fi
  SYSNICMASK="$ANS"

  get_dlg_ans "--inputbox \"Enter the DNS address for $SYSNIC\" 8 40"
  if [ -z "$ANS" ] ; then
     exit_err "Invalid real name entered!"
  fi
  SYSNICDNS="$ANS"

  get_dlg_ans "--inputbox \"Enter the Gateway address for $SYSNIC\" 8 40"
  if [ -z "$ANS" ] ; then
     exit_err "Invalid real name entered!"
  fi
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
   echo "disk0-part=ZFS 0 ${ZFSLAYOUT}" >> ${CFGFILE}
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
   fi

   # Last cleanup stuff
   echo "" >> ${CFGFILE}
   echo "runExtCmd=/root/save-config.sh" >> ${CFGFILE}
   echo "runCommand=newaliases" >> ${CFGFILE}

   # Are we enabling SSHD?
   if [ "$SYSSSHD" = "YES" ] ; then
     echo "runCommand=echo 'sshd_enable=\"YES\"' >> /etc/rc.conf" >> ${CFGFILE}
   fi

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
  fi
  gen_pc-sysinstall_cfg
}

# Adjust network options
change_networking() {
  get_hostname
  get_netconfig
  get_sshd
  gen_pc-sysinstall_cfg
}

start_edit_menu_loop()
{

  while :
  do
    dialog --title "PC-BSD Text Install - Edit Menu" --menu "Please select from the following options:" 18 40 10 disk "Change disk ($SYSDISK)" zfs "Change ZFS layout" network "Change networking" view "View install script" edit "Edit install script" back "Back to main menu" 2>/tmp/answer
    if [ $? -ne 0 ] ; then break ; fi

    ANS="`cat /tmp/answer`"

    case $ANS in
       disk) change_disk_selection
             rtn
             ;;
    network) change_networking 
	     ;;
        zfs) change_zfs
	     ;;
       view) more ${CFGFILE}
             rtn
             ;;
       edit) vi ${CFGFILE}
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
    dialog --title "PC-BSD Text Install" --menu "Please select from the following options:" 18 40 10 install "Start the installation" wizard "Re-run install wizard" edit "Edit install options" quit "Quit install wizard" 2>/tmp/answer
    if [ $? -ne 0 ] ; then break ; fi

    ANS="`cat /tmp/answer`"

    case $ANS in
     wizard) start_full_wizard
             rtn
             ;;
       edit) start_edit_menu_loop
             ;;
    install) echo "This will begin the installation, continue?"
             echo -e "(y/n)\c"
             read tmp
             if [ "$tmp" = "y" -o "$tmp" = "Y" ] ; then
                ${PCSYS} -c ${CFGFILE}
                rtn
             fi
             ;;
       quit) break ;;
          *) ;;
    esac
  done

}



if [ ! -e "$CFGFILE" ] ; then
   cp ${CFGFILE} ${CFGFILE}.bak
   rm ${CFGFILE}
fi

start_full_wizard
start_menu_loop

exit 0
