#!/bin/sh
# Cron script which checks preservers and starts backups

PATH="${PATH}:/usr/local/bin:/usr/local/sbin"
export PATH

PROGDIR="/usr/local/share/lifePreserver"
PDIRS="${PROGDIR}/preservers/"

# Source our functions
. ${PROGDIR}/scripts/functions.sh

cyear=`date +%y`
cmon=`date +%m`
cday=`date +%d`

for p in `ls ${PDIRS}`
do
	if [ ! -e "${PDIRS}/${p}/frequency" ] ; then continue; fi
	freq="`cat ${PDIRS}/${p}/frequency`"
	if [ "$freq" != "daily" -a "$freq" != "weekly" ] ; then continue; fi


	# Check if we need to do a backup
	nb="NO"
	if [ ! -e "${PDIRS}/${p}/last-timestamp" ] ; then
		nb="YES"
	else
		year=`cat ${PDIRS}/${p}/last-timestamp | cut -d ':' -f 1`
		mon=`cat ${PDIRS}/${p}/last-timestamp | cut -d ':' -f 2`
		day=`cat ${PDIRS}/${p}/last-timestamp | cut -d ':' -f 3`
		year=`expr $year \* 365`
		cyear=`expr $cyear \* 365`
		mon=`expr $mon \* 30`
		cmon=`expr $cmon \* 30`
		num1=`expr $cyear + $cmon + $cday`
		num2=`expr $year + $mon + $day`
		count=`expr $num1 - $num2`
		if [ "$freq" = "daily" ] ; then
			if [ $count -ge 1 ] ; then nb="YES"; fi
		else
			if [ $count -ge 7 ] ; then nb="YES"; fi
		fi
	fi
	
	# If yes, lets start a background backup process
	if [ "$nb" = "YES" ] ; then
		(${PROGDIR}/scripts/start-backup.sh ${p} >/dev/null 2>/dev/null) &
	fi

done
