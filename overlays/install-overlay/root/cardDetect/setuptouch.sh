#!/bin/sh
# Script to look for touch-screen usbhid devices and add them to xorg.conf

# Set location of xorg.conf we are working on
TMPFILE="$1"

##############################################################################
# Check for any touch-screen devices

for uh in `ls /dev/uhid* 2>/dev/null`
do
  touch=0
  tcount=`usbhidctl -f $uh -r | \
  egrep 'Collection( type=Application)? page=(Generic_Desktop|Digitizer)' | \
  grep -e 'usage=Mouse' \
  -e 'usage=Digitizer' \
  -e 'usage=Pen' \
  -e 'usage=Light_Pen' \
  -e 'usage=Touch_Screen' \
  -e 'usage=Touch_Pad' \
  -e 'usage=White_board' \
  -e 'usage=Coordinate_Measuring_Machine' \
  -e 'usage=3D_Digitizer' \
  -e 'usage=Stereo_Plotter' \
  -e 'usage=Articulated_Arm' \
  -e 'usage=Armature' \
  -e 'usage=Multiple_Point_Digitizer' \
  -e 'usage=Free_Space_Wand' \
  | wc -l | awk '{print $1}'`

  # Make sure we have a valid number
  expr $tcount + 1 2>/dev/null
  if [ $? -ne 0 ] ; then continue ; fi

  # Any touch devices?
  if [ $tcount -eq 0 -o $tcount -lt 0 ] ; then continue ; fi

  # Add xorg.conf entries
  added=0
  while read line
  do
    echo $line | grep -q 'Mouse0'
    if [ $? -eq 0 ] ; then
      if [ $added -ne 0 ] ; then
        echo -e "${line}" >> ${TMPFILE}.new
      else
        echo -e "${line}" >> ${TMPFILE}.new
        snum=0
        while
        z=1
        do
          echo "InputDevice    \"Panel${snum}\"" >> ${TMPFILE}.new
          snum=`expr $snum + 1`
          if [ $snum -eq $tcount ] ; then added=1; break ; fi
        done
        added=1
      fi
    else
      echo -e "${line}" >> ${TMPFILE}.new
    fi
  done < ${TMPFILE}
  mv ${TMPFILE}.new ${TMPFILE}

  # Now add InputDevice Sections
  snum=0
  while
  z=1
  do
    
    # Add the InputDevice
    echo "
Section \"InputDevice\"
Identifier      \"Panel${snum}\"
Driver          \"mouse\"
Option          \"Protocol\" \"usb\"
Option          \"Device\" \"/dev/uhid0\"
EndSection" >>${TMPFILE}

    snum=`expr $snum + 1`
    if [ $snum -eq $tcount ] ; then break ; fi

  done 

  # Found a touch-device, break out now
  break
done


##############################################################################
