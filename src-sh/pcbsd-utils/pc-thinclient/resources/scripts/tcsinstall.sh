#!/bin/sh
# Script to provide menu-driven installation of systems

do_installwiz() {

  installChoices=""
  for i in `ls /installscripts/*.cfg`
  do
     installChoices="$installChoices $i $i"
  done

  dialog --menu 'Select install script' 15 55 10 'back' 'Go back' ${installChoices} 2>/tmp/ans
  ANS=`cat /tmp/ans`
  case $ANS in
       back) return ;;
	*) echo "Ready to begin installation using command:"
	   echo "/usr/local/sbin/pc-sysinstall -c /installscripts/${ANS}" 
	   echo -e "Continue? (y/n) \c"
	   read tmp
	   if [ "$tmp" = "y" -o "$tmp" = "Y" ] ; then
	      /usr/local/sbin/pc-sysinstall -c /installscripts/${ANS}
	   fi
	   ;;
  esac
}

# Function which determines if a nic is active / up
is_nic_active()
{
  ifconfig ${1} | grep -q "status: active" 2>/dev/null
  if [ $? -eq 0 ] ; then
    return 0
  else
    return 1
  fi
}

# Find the active nic
get_active_nic()
{
  for i in $(ifconfig -l); do
    case "${i%%[0-9]*}" in
      lo|fwe|fwip|plip|pfsync|pflog|tun)
      continue
      ;;
    esac
    is_nic_active "${i}"
    if [ $? -eq 0 ] ; then
       echo "$i"
       return 0
    fi
  done
  return 1
}

# Get the mac address of a target NIC
get_nic_mac()
{
  FOUNDMAC="`ifconfig ${1} | grep 'ether' | tr -d '\t' | cut -d ' ' -f 2`"
  echo $FOUNDMAC
}

# Check for MAC address config file
NIC=$(get_active_nic)
MAC=$(get_nic_mac "$NIC")

# Check if the mac address is in the blacklist
# Create this file with a line-by-line list of MACs to blacklist
if [ -e "/installscripts/macblacklist" ] ; then
   grep -q "^${MAC}" /installscripts/macblacklist
   if [ $? -eq 0 ] ; then
      echo "ERROR: This MAC address listed in the /installscripts/macblacklist file!"
      echo "To install this system, remove the address: ${MAC}"
      echo ""
      echo "Installation HALTED!"
      echo "[Press Enter to shutdown]"
      shutdown -p now
   fi
fi

# Detect auto-install scripts
ASCRIPT=""
if [ -e "/installscripts/unattended.cfg" ] ; then
   ASCRIPT="/installscripts/unattended.cfg"
fi
if [ -e "/installscripts/${MAC}.cfg" ] ; then
   ASCRIPT="/installscripts/${MAC}.cfg"
fi

# Are we doing an automated install?
if [ -n "${ASCRIPT}" ] ; then

  echo "Preparing to install using: ${ASCRIPT}"
  echo "Will automatically continue in 30 seconds."
  echo "Press ENTER to cancel"

  read -t30s ans
  if [ $? -eq 1 ] ; then
    pc-sysinstall -c ${ASCRIPT}
    if [ $? -eq 0 ] ; then
      echo "Install finished! Shutting down system now!"
      halt -p
      exit 0
    fi

    echo "Install failed! Full log of installation: /tmp/.pc-sysinstall/pc-sysinstall.log" 
    echo "Press ENTER to continue"
    read tmp
  fi
fi

# Run the interactive menu
while :
do
  dialog --menu 'Select install option' 15 55 10 'install' 'Start install wizard' 'shell' 'Run emergency shell' 'close' 'Close and Reboot' 2>/tmp/ans
  ANS=`cat /tmp/ans`
  case $ANS in
   install) do_installwiz
            echo "[Press Enter to Continue]"
            read tmp
            ;;
     shell) /bin/csh
            echo "[Press Enter to Continue]"
            read tmp
            ;;
     close) reboot
	    exit
            ;;
         *) ;;
   esac
done


