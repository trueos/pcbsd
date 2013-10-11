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

  # Ignore X -configure for now, let startx handle it
  # Now run the X auto-detection
  #echo "Detecting X Setup..."
  #sleep 5
  #HOME="/root" ; export HOME
  #X -configure >/dev/null 2>&1
  # Copy over the new xorg.conf
  #cp /root/xorg.conf.new /etc/X11/xorg.conf
  # Use nvidia driver
  #sed -i '' 's|"nv"|"nvidia"|g' /etc/X11/xorg.conf

}

start_xorg()
{
  # Run X Now
  startx
  if [ ! -e "/tmp/.xstarted" ]
  then
    # Failed to start X with detected driver. Now try our detection mode.
    echo "ERROR: Failed to start X with detected driver... Trying SAFE mode..."
    rm /etc/X11/xorg.conf
    X -configure >/dev/null 2>&1
    # Copy over the new xorg.conf
    cp /root/xorg.conf.new /etc/X11/xorg.conf

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
