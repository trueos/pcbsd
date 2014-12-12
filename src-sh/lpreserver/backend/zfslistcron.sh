#!/bin/sh
# ZFS functionality
# Args $1 = jail-name
# Args $2 = zfs directive
#######################################################################

# Source our functions
PROGDIR="/usr/local/share/lpreserver"

# Source our variables
. /usr/local/share/pcbsd/scripts/functions.sh
. ${PROGDIR}/backend/functions.sh

if [ -z "$1" ]; then
    list_cron_snap
    list_cron_scrub
    exit 0
fi

if [ "$1" == "snap" ]; then
    list_cron_snap
    exit 0
fi

if [ "$1" == "scrub" ]; then
    list_cron_scrub
    exit 0
fi
