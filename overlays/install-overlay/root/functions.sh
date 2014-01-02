#!/bin/sh
# Functions we source for starting the install / live mode
###########################################################

detect_x() 
{
  # Check for VESA support
  xvesa="NO"
  v=`/bin/kenv xvesa 2>/dev/null`
  if [ $? -eq 0 ]; then
        xvesa=$v
  fi
  # If we are starting in VESA only mode
  if [ "$xvesa" = "YES" ]; then
    cp /root/cardDetect/XF86Config.compat /etc/X11/xorg.conf
    return
  fi

}

cfg_second_card()
{
  # This is a script to try an xorg.conf file configured to use the second vgapci1 device
  # For most systems this wont do anything, but on a number of newer hybrid
  # intel/amd or intel/nvidia laptops this may fix a problem where the intel card (which works)
  # is the secondary pcivga1 device, and the non-functional AMD/NVIDIA optimus  shows up first.

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

  # No secondary card, return 1
  if [ -z "$card2" ] ; then return 1; fi

  # Found a second card, lets try an xorg config for it
  cp /root/cardDetect/XF86Config.default /etc/X11/xorg.conf
  echo "
Section \"Device\"
        Identifier      \"Card0\"
        BusID           \"${card2bus}\"
EndSection
  " >> /etc/X11/xorg.conf

  return 0

}

start_xorg()
{
  # Run X Now
  startx
  if [ ! -e "/tmp/.xstarted" ]
  then
    # Failed to auto-start X with its own internal video detection
    # Now lets try some magic
    rm /etc/X11/xorg.conf 2>/dev/null

    echo "ERROR: Failed to start X with default video card... Trying secondary mode..."
    cfg_second_card
    if [ $? -ne 0 ] ; then
      # Try the Intel driver, since nvidia/vesa will fail on optimus cards
      cp /root/cardDetect/XF86Config.intel /etc/X11/xorg.conf
    fi

    startx
    if [ ! -e "/tmp/.xstarted" ]
    then
      echo "ERROR: Failed to start X in SAFE mode... Trying VESA mode..."
      rm /etc/X11/xorg.conf
      cp /root/cardDetect/XF86Config.compat /etc/X11/xorg.conf
      startx
     if [ ! -e "/tmp/.xstarted" ]
      then
        # Try the Intel driver, since nvidia/vesa will fail on optimus cards
        cp /root/cardDetect/XF86Config.intel /etc/X11/xorg.conf
        startx
        if [ ! -e "/tmp/.xstarted" ]
        then
          echo "ERROR: Failed to start X..."
          echo "Dropping to failsafe console... Edit /etc/X11/xorg.conf, and run #startx to bring up the GUI."
          echo "[Press Enter to Continue]"
          read tmp
          /bin/sh
        fi
      fi
    fi
  fi

}

rtn()
{
   echo "Press ENTER to continue"
   read tmp
}
