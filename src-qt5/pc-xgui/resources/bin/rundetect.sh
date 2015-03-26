#!/bin/sh
# xorg-gui startup script
# Copyright 2009 PC-BSD Software
# Descr: This script is called by /usr/local/etc/rc.d/gdm to run the time X setup
#        wizard. It allows the user to select a resolution / refresh rate and
#        test it before continuing into the normal KDE GUI.
###############################################################################
# Configuration Settings

# Set all our path variables
PATH="/sbin:/bin:/usr/sbin:/usr/bin:/root/bin:/usr/local/bin:/usr/local/sbin"
export PATH

# Set the home directory for startx to run properly
HOME="/root" ; export HOME

# Set the log file location
XLOG="/tmp/xsetup.log" ; export XLOG

# Program Directory
PROGDIR="/usr/local/share/pcbsd/xorg-gui" ; export PROGDIR

XGUITMP="/tmp/.xgui" ; export XGUITMP

TERM="cons25" ; export TERM
clear

###############################################################################
# Start the script now

cfg_card_busid()
{
  whichcard="$1"

  # This script will manually set BusID for the first or second vgapciX device
  # For some reason in 10.0, it no longer auto-probes for different VGA devices :(
  inCard=0
  pciconf -lv > /tmp/.pciconf.$$
  while read line
  do
    echo $line | grep -q "^vgapci"
    if [ $? -eq 0 ] ; then
       curCard=`echo $line | cut -d "@" -f 1 | sed 's|vgapci||g'`
       busID="`echo $line | cut -d ':' -f 2-4`"
       inCard=1
       continue
    fi

    echo $line | grep -q "subclass"
    if [ $? -eq 0 ] ; then inCard=0; continue; fi

    if [ $inCard -eq 1 ] ; then
       echo $line | grep -q "vendor"
       if [ $? -eq 0 ]; then
          case $curCard in
             0) card1=`echo $line | cut -d "'" -f 2`
                card1bus="$busID"
                ;;
             1) card2=`echo $line | cut -d "'" -f 2`
                card2bus="$busID"
                ;;
             *) ;;
          esac
       fi
    fi
  done < /tmp/.pciconf.$$
  rm /tmp/.pciconf.$$

  # Check if we need to "flip" the cards, I.E. the Intel video should be the card #1, all others #2
  echo "$card2" | grep -q -i -e "intel"
  if [ $? -eq 0 ] ; then
     tmpCard="$card1"
     tmpCardBus="$card1bus"
     card1="$card2"
     card1bus="$card2bus"
     card2="$tmpCard"
     card2bus="$tmpCardBus"
  fi

  # Which card are we configuring
  if [ "$whichcard" = "1" ] ; then
    cfgCard="$card1"
    cfgCardBusID="$card1bus"
  else
    cfgCard="$card2"
    cfgCardBusID="$card2bus"
  fi

  # No detected, return 1
  if [ -z "$cfgCard" ] ; then return 1; fi

  # Is this an Intel chipset?
  echo "$cfgCard" | grep -q -i -e "intel"
  if [ $? -eq 0 ] ; then
     driver="intel"
  fi
  echo "$cfgCard" | grep -q -i -e "nvidia"
  if [ $? -eq 0 ] ; then
     driver="nvidia"
  fi

  # Found a card, lets try an xorg config for it
  cp ${PROGDIR}/cardDetect/XF86Config.default /etc/X11/xorg.conf
  if [ -n "$driver" ] ; then
    echo "
Section \"Device\"
        Identifier      \"Card0\"
        Driver          \"$driver\"
        BusID           \"${cfgCardBusID}\"
EndSection
  " >> /etc/X11/xorg.conf
  else
    echo "
Section \"Device\"
        Identifier      \"Card0\"
        BusID           \"${cfgCardBusID}\"
EndSection
  " >> /etc/X11/xorg.conf
  fi

  return 0
}

# Init our tmpdir
if [ ! -d "${XGUITMP}" ] ; then
	mkdir -p ${XGUITMP}
fi

echo "`clear`" >/dev/console

# Move any existing xorg.conf file
if [ -e "/etc/X11/xorg.conf" ] ; then
   mv /etc/X11/xorg.conf /etc/X11/xorg.conf.bak
fi

echo "`clear`" >/dev/console
echo "Please wait... Start X-Configuration Utility" >/dev/console

# Check if we need to save an existing .xinitrc file
if [ -e "/root/.xinitrc" ] ; then
 mv /root/.xinitrc /root/.xinitrc.xbak
fi

# Check if we need to move the .fluxbox profile
if [ -e "/root/.fluxbox" ] ; then
  rm -rf /root/.fluxbox.xbak 2>/dev/null
  mv /root/.fluxbox /root/.fluxbox.xbak 2>/dev/null
fi

rm -rf /root/.fluxbox
cp -R ${PROGDIR}/conf/.fluxbox /root/.fluxbox

cd /root/


# Recreate our log file
if [ -e "${XLOG}" ] ; then
  mv ${XLOG} ${XLOG}.old
fi

# Create an empty log file for appending
echo "" > ${XLOG}


# Remove any files from a previous attempt
rm ${XGUITMP}/.canceled >/dev/null 2>/dev/null
rm ${XGUITMP}/.selected >/dev/null 2>/dev/null
rm ${XGUITMP}/.rerunxsetup >/dev/null 2>/dev/null
rm ${XGUITMP}/.done >/dev/null 2>/dev/null
rm ${XGUITMP}/.failed >/dev/null 2>/dev/null


# Set the attempt number to 0
ATTEMPT="0"

###############################################################################
# Start the detection loop

while
i=1
do

  # Save the previous xorg.conf file for troubleshooting
  if [ -e "/etc/X11/xorg.conf" ] ; then mv /etc/X11/xorg.conf /etc/X11/xorg.conf.previous ; fi

  #  Don't keep displaying the autores question
  AUTORES="NO"

  if [ "${ATTEMPT}" = "0" ] ; then

    # First check if we are running as a VirtualBox guest
    pciconf -lv | grep -q "VirtualBox"
    if [ $? -eq 0 ] ; then cp ${PROGDIR}/cardDetect/xorg.conf.virtualbox /etc/X11/xorg.conf; fi

    # Check if we are supposed to run in vesa mode
    xvesa="NO"
    v=`kenv xvesa`
    if [ $? -eq 0 ]; then xvesa=$v ; fi

    # Check if user requested to run in vesa mode specifically and do it
    if [ "$xvesa" = "YES" ] ; then
      echo "`clear`" >/dev/console
      echo "Using failsafe VESA 1024x768 mode..." >/dev/console
      cp ${PROGDIR}/cardDetect/XF86Config.compat /etc/X11/xorg.conf
    else
      cfg_card_busid "1"
      AUTORES="YES"
    fi
  elif [ "${ATTEMPT}" = "1" ] ; then

    # Try BUSID of the second card
    cfg_card_busid "2"
    echo "`clear`" >/dev/console
    echo "ERROR: Failed to start X with default video card... Trying secondary..." >/dev/console

  elif [ "${ATTEMPT}" = "2" ] ; then

    # Try INTEL mode
    cp ${PROGDIR}/cardDetect/XF86Config.intel /etc/X11/xorg.conf
    echo "`clear`" >/dev/console
    echo "ERROR: Trying INTEL compat mode..." >/dev/console

  elif [ "${ATTEMPT}" = "3" ] ; then

    # Check if this system has a nvidia device, and run nvidia-xconfig
    rm /etc/X11/xorg.conf
    kldstat | grep -q 'nvidia'
    if [ $? -eq 0 ] ; then
       nvidia-xconfig 2>/dev/null
       echo "`clear`" >/dev/console
       echo "ERROR: Trying NVIDIA automatic mode..." >/dev/console
    else
       echo "`clear`" >/dev/console
       echo "ERROR: Trying XORG automatic mode..." >/dev/console
    fi
  elif [ "${ATTEMPT}" = "4" ] ; then

    # Still failed, drop to VESA failsafe
    rm /etc/X11/xorg.conf
    cp ${PROGDIR}/cardDetect/XF86Config.compat /etc/X11/xorg.conf
    echo "`clear`" >/dev/console
    echo "Detected settings failed... Using failsafe VESA 1024x768 mode..." >/dev/console

  else

    # Nothing we tried worked, so let the user decide on their own config
    echo "Failed starting X with auto-detected configuration..."
    echo "Please login on an alternative terminal (Ctrl-Alt-F2)"
    echo "and edit /etc/X11/xorg.conf"
    echo ""
    echo "When you are finished please return to this console:"
    echo "(Ctrl-Alt-F1) and press ENTER to start X again"
    read tmp
  fi

  # Check if the previous attempt failed
  if [ -e "${XGUITMP}/.failed" ]
  then
  # Failed display, show the failed screen
    rm ${XGUITMP}/.failed
    cp ${PROGDIR}/scripts/xinit-failed /root/.xinitrc
    chmod 755 /root/.xinitrc
  elif [ "$AUTORES" = "YES" ] ; then
  # First time running, ask user if they want to use auto-detected resolution
    cp ${PROGDIR}/scripts/xinit-autodetect /root/.xinitrc
    chmod 755 /root/.xinitrc
  else
  # Did not fail, create normal config
    cp ${PROGDIR}/scripts/xinit-displaywiz /root/.xinitrc
    chmod 755 /root/.xinitrc
  fi

  echo "`clear`" >/dev/console
  echo "Please wait... Starting X..." >/dev/console
  sleep 1
  # Start the X gui
  /usr/local/bin/startx 2>>${XLOG}

  # The user canceled the dialog, and chose to use the default
  if [ -e "${XGUITMP}/.canceled" ] ; then break ; fi

  # Check if a new version of xorg.conf is ready to be tested
  if [ -e "${XGUITMP}/.xsettings.sh" ]
  then
    rm ${XGUITMP}/.selected >/dev/null 2>/dev/null

    #### Create the new XF86Config file with the user settings
    echo "`clear`" >/dev/console
    echo "Generating new Xorg configuration file..." >/dev/console
    ${PROGDIR}/bin/setupconf.sh >>${XLOG} 2>>${XLOG}

    ##### Copy the xinitrc file
    cp ${PROGDIR}/scripts/xinit-check /root/.xinitrc
    chmod 755 /root/.xinitrc

    echo "`clear`" >/dev/console
    echo "Please wait... Starting X..." >/dev/console
    sleep 1
    /usr/local/bin/startx 2>>${XLOG}
  fi

  if [ -e "${XGUITMP}/.selected" ] ; then
    break
  else
    echo "failed" > ${XGUITMP}/.failed 
  fi

  # Check if the X startup was a success
  if [ ! -e "${XGUITMP}/.xstartupsuccess" ] ; then
    ATTEMPT="`expr $ATTEMPT + 1`"
    rm ${XGUITMP}/.failed
  else
    rm ${XGUITMP}/.xstartupsuccess
  fi

  # Removed any saved settings file
  rm ${XGUITMP}/.xsettings.sh

done


rm /root/.xinitrc >/dev/null 2>/dev/null
# Check if we need to save an existing .xinitrc file
if [ -e "/root/.xinitrc.xbak" ] ; then
   mv /root/.xinitrc.xbak /etc/.xinitrc
fi

# Check if we need to move the .fluxbox profile
if [ -e "/root/.fluxbox.xbak" ] ; then
  rm -rf /root/.fluxbox >/dev/null 2>/dev/null
  mv /root/.fluxbox.xbak /root/.fluxbox
fi

rm ${XGUITMP}/.rerunxsetup >/dev/null 2>/dev/null
rm ${XGUITMP}/.skipauto >/dev/null 2>/dev/null
echo "`clear`" >/dev/console
echo "Preparing to start the desktop..." >/dev/console
sleep 1
exit 0
