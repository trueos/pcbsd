#!/bin/sh
# Functions we source for starting the install / live mode
###########################################################

detect_x() 
{
  # Check if the user requested VESA mode
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

  # First check if we are running as a VirtualBox guest
  pciconf -lv | grep -q "VirtualBox"
  if [ $? -eq 0 ] ; then cp /root/cardDetect/xorg.conf.virtualbox /etc/X11/xorg.conf ; return; fi

  # Try the second card, since many optimus / amd hybrids have Intel on secondary
  cfg_card_busid "1"

}

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
  cp /root/cardDetect/XF86Config.default /etc/X11/xorg.conf
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

start_xorg()
{

  # Now run the X auto-detection
  detect_x

  # Run X Now
  startx
  if [ ! -e "/tmp/.xstarted" ]
  then
    # Failed to start X
    # Lets try again with a secondary video card
    rm /etc/X11/xorg.conf 2>/dev/null
    cfg_card_busid "2"

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

zpool_import()
{
  echo "Available zpools:"
  zpool import | grep "pool: " | awk '{print $2}'
  echo "--------------------------------"
  echo -e "Please enter the pool name to import:\c"
  read mypool

  zpool import -f -N $mypool
  if [ $? -ne 0 ] ; then
     echo "Failed to import pool!"
     rtn
     return 1
  fi

  # Now try to mount the root dataset
  mount -t zfs ${mypool}/ROOT/default /mnt
  if [ $? -ne 0 ] ; then
     lastRoot=`zfs list -H | grep "$mypool/ROOT/" | awk '{print $1}' | tail -n 1`
     mount -t zfs ${lastRoot} /mnt
     if [ $? -ne 0 ] ; then
       echo "Failed to mount root dataset! Please manually mount to /mnt"
       rtn
       return 1
     fi
  fi

  zfs list -H | tr -s '\t' ' ' | grep -v "${mypool}/ROOT/" | grep -v "$mypool " | grep -v " /mnt" > /tmp/.mntList.$$

  while read line
  do
     dset=`echo $line | awk '{print $1}'`
     lmnt=`echo $line | awk '{print $5}'`
     mount -t zfs ${dset} /mnt/${lmnt}
     if [ $? -ne 0 ] ; then
       echo "Warning: Failed to mount: $dset to /mnt/$lmnt"
       sleep 1
     fi
  done < /tmp/.mntList.$$
  rm /tmp/.mntList.$$

  # Mount devfs into the zpool lastly
  mount -t devfs devfs /mnt/dev

  echo "Finished mounting zpool: $mypool"

  # If function run without chroot prompt requested
  if [ "$1" = "nochroot" ] ; then return 0 ; fi

  echo -e "Open a chroot shell now? (y/n): \c"
  read tmp
  if [ "$tmp" = "y" -o "$tmp" = "Y" ] ; then
    chroot /mnt
  else
    echo "Use the command 'chroot /mnt' to switch to the pool root"
  fi

  rtn
  return 0
}

restamp_grub_install()
{
  mount | grep -q "on /mnt"
  if [ $? -ne 0 ] ; then
     zpool_import "nochroot"
     if [ $? -ne 0 ] ; then
        echo "Failed importing zpool! Please import a pool before doing grub-install."
        rtn
        return
     fi
  fi

  # Copy over the installer version of beadm
  cp /root/beadm.install /mnt/root/beadm.install
  if [ $? -ne 0 ] ; then
     echo "Failed copying beadm.install..."
     return 1
  fi

  # Run the grub restamp now
  chroot /mnt restamp-grub
  if [ $? -ne 0 ] ; then
     echo "Failed running restamp-grub..."
     rm /mnt/root/beadm.install
     return 1
  fi

  # Remove installer version of beadm.install
  rm /mnt/root/beadm.install

  return 0
}
