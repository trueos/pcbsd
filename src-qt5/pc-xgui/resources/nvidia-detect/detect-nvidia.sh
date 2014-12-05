#!/bin/sh
# Script which checks the nvidia cards PCIID and figures out which driver it needs
##################################################################################
PROGDIR="/usr/local/share/pcbsd/xorg-gui"

NV17FILE="${PROGDIR}/nvidia-detect/nvidia17-pciid.txt"
NV9XFILE="${PROGDIR}/nvidia-detect/nvidia9x-pciid.txt"
NV17DRV="`grep nvidia-17 ${PROGDIR}/settings/drivers.txt`"
NV9XDRV="`grep nvidia-9 ${PROGDIR}/settings/drivers.txt`"


ID="`${PROGDIR}/scripts/cardpciid.sh`"

# First check the 17 version
grep "^${ID}" ${NV17FILE} >/dev/null 2>/dev/null
if [ "$?" = "0" ]
then
  echo "$NV17DRV"
  exit 0
fi

# Now check the 9X version
grep "^${ID}" ${NV9XFILE} >/dev/null 2>/dev/null
if [ "$?" = "0" ]
then
  echo "$NV9XDRV"
  exit 0
fi

# No driver match, let tool know
echo "UNKNOWN"

