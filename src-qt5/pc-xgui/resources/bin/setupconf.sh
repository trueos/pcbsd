#!/bin/sh

XGUITMP="/tmp/.xgui" ; export XGUITMP

# Source the settings file
chmod 755 ${XGUITMP}/.xsettings.sh
. ${XGUITMP}/.xsettings.sh

# Program Directory
PROGDIR="/usr/local/share/pcbsd/xorg-gui" ; export PROGDIR

# Set the tmp xorg file
TMPFILE="${XGUITMP}/.xorg.conf.tmp"

# Get the real driver name and save the value from xorg-gui
SELDRIVER="${DRIVER}" ; export SELDRIVER
grep "^${DRIVER}:" ${PROGDIR}/settings/alias.txt >/dev/null 2>/dev/null
if [ "$?" = "0" ]
then
  DRIVER=`cat ${PROGDIR}/settings/alias.txt | grep "^${DRIVER}:" | cut -d ":" -f 2`
  export DRIVER
else
  echo "Warning: Selected driver not found in alias.txt file..."
fi

# Check if the driver has a special header to use in place of the generic one
if [ -e "${PROGDIR}/templates/header/${DRIVER}.xorg.conf" ]
then
  cp "${PROGDIR}/templates/header/${DRIVER}.xorg.conf" $TMPFILE 
else
  cp ${PROGDIR}/templates/header/xorg.conf.generic $TMPFILE 
fi


# Get the names for the Monitor Model / Vendor
###########################################################################################

MONMODEL="`${PROGDIR}/scripts/monmodel.sh`"
if [ -z "${MONMODEL}" ]
then
   MONMODEL="Generic Monitor"
fi

MONVENDOR="`${PROGDIR}/scripts/monvendor.sh`"
if [ -z "${MONVENDOR}" ]
then
   MONVENDOR="Generic Monitor"
fi
##########################################################################################



# Setup our Monitor Section, loop if we are using dual-head
##########################################################################################
i=0
if [ -z "${DUALHEAD}" ]
then
   totalMon=1;
else
   totalMon=2;
fi


# Start the loop now
while
z=1
do

# Check if we have finished the last monitor
if [ "${i}" = "${totalMon}" -o ${i} -gt 1 ]
then
   break
fi

# Setup the monitor section with GTF values
echo "
Section \"Monitor\"
    Identifier     \"Monitor${i}\"
    VendorName     \"${MONVENDOR}\"
    ModelName      \"${MONMODEL}\"" >> $TMPFILE

if [ "${HORIZSYNC}" != "OFF" ]
then

echo "    
    HorizSync    ${HORIZSYNC}
    VertRefresh  ${VERTREFRESH}" >> ${TMPFILE}

else
  # The user didnt specify values so check if we have auto-detected ones
  # Check for refresh rate lines
  cat /root/xorg.conf.new | grep HorizSync >/dev/null 2>/dev/null
  if [ "$?" = "0" ]
  then
    cat /root/xorg.conf.new | grep HorizSync >>${TMPFILE}
  fi

  cat /root/xorg.conf.new | grep VertRefresh >/dev/null 2>/dev/null
  if [ "$?" = "0" ]
  then
    cat /root/xorg.conf.new | grep VertRefresh >>${TMPFILE}
  fi
fi

echo 'EndSection

' >> $TMPFILE

i="`expr ${i} + 1`"

done # End of our while loop for the monitor setup

#####################################################################################


# Check if on dual-head, and setup the ServerFlags to use it
####################################################################################
if [ ! -z "${DUALHEAD}" ]
then
  if [ "${DRIVER}" = "nvidia" ]
  then
     echo 'Section "ServerFlags"
    Option         "Xinerama" "0"
EndSection

' >> ${TMPFILE}
  else
     echo 'Section "ServerFlags"
    Option         "Xinerama" "1"
EndSection

' >> ${TMPFILE}
  fi
fi

###################################################################################

# Setup our "Device" section, with dual-head if enabled
###################################################################################

i=0
if [ -z "${DUALHEAD}" ]
then
   totalDev=1;
else
   totalDev=2;
fi


# Start the loop now
while
z=1
do

# Check if we have finished the last Device
if [ "${i}" = "${totalDev}" -o ${i} -gt 1 ]
then
   break
fi

echo "Section \"Device\"
    Identifier      \"Card${i}\"
    Screen           ${i}" >> ${TMPFILE}


# Copy over the defined template options for the specific driver
cat ${PROGDIR}/templates/driver/${DRIVER}.template >>$TMPFILE

# Check if we have a cardbus variable
CARDBUS="`${PROGDIR}/scripts/cardbus.sh`"

if [ ! -z "${CARDBUS}" ]
then
   echo "    BusID      \"${CARDBUS}\"" >> ${TMPFILE}
fi

echo "EndSection
" >> ${TMPFILE}

i="`expr ${i} + 1`"

done # End of while loop for Device Section

##################################################################################

# Now create the Screen section, loop since we may be using dual-head
##################################################################################

i=0
if [ -z "${DUALHEAD}" ]
then
   totalScreen=1;
else
   totalScreen=2;
fi


# Start the loop now
while
z=1
do

# Check if we have finished the last Device
if [ "${i}" = "${totalScreen}" -o ${i} -gt 1 ]
then
   break
fi

# Check which monitor we are on, and use the right resolution for it
case ${i} in
   0) MONRES="${RES}" ;;
   1) MONRES="${DUALRES}" ;;
   *) MONRES="${RES}" ;;
esac

# Start adding the screen entry now
echo "
Section \"Screen\"
    Identifier     \"Screen${i}\"
    Device         \"Card${i}\"
    Monitor        \"Monitor${i}\"
    DefaultDepth   ${DEPTH}
    SubSection     \"Display\"
        Depth      ${DEPTH} 
        Modes      \"${MONRES}\"
    EndSubSection" >> $TMPFILE

# If we are on dualhead with NVIDIA, add the twinview stuff
if [ ! -z "${DUALHEAD}" -a "${DRIVER}" = "nvidia" ]
then
   if [ "${i}" = "0" ]
   then
     echo "    Option         \"TwinView\" \"1\"" >>${TMPFILE}
     WIDTH="`echo ${RES} | cut -d 'x' -f 1`"
     echo "    Option         \"metamodes\" \"DFP-0: ${RES} +0+0, DFP-1: ${RES} +${WIDTH}+0\"" >>${TMPFILE}
   else
     echo "    Option         \"TwinView\" \"0\"" >>${TMPFILE}
     echo "    Option         \"metamodes\" \"DFP-${i}: ${MONRES} +0+0\"" >>${TMPFILE}
   fi
fi

echo "EndSection
" >> $TMPFILE

i="`expr ${i} + 1`"

done # End of while loop for the Screen device

##############################################################################

mv $TMPFILE /etc/X11/xorg.conf

# Done generating the xorg.conf file
##############################################################################


# Check if we need to remove any nvidia specific settings
if [ "${DRIVER}" != "nvidia" ]
then
	#Check if the driver is already loaded
	kldstat | grep nvidia >/dev/null 2>/dev/null
	if [ "${?}" = "0" ] ; then kldunload nvidia ; fi

	cat /boot/loader.conf | grep nvidia_load >/dev/null 2>/dev/null
	if [ "${?}" = "0" ] ; then
  		cat /boot/loader.conf | grep -v 'nvidia_load' > /boot/loader.conf.new
  		mv /boot/loader.conf.new /boot/loader.conf
	fi

	# Make sure we enable built-in AGP
	cat /boot/device.hints | grep -v "hint.agp.0.disabled" >/tmp/.device.hints
	mv /tmp/.device.hints /boot/device.hints

fi # End of nvidia driver not selected check


# See if this driver needs any post-setup operations to run
if [ -e "${PROGDIR}/templates/scripts/${DRIVER}.sh" ]
then
  # We found the file, so run it
  sh "${PROGDIR}/templates/scripts/${DRIVER}.sh" "${SELDRIVER}"
fi

