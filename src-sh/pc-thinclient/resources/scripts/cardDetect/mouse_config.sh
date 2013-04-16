#!/bin/sh
#
#This script was taken from Frenzy Live CD developed by Sergei Mozhaisky
# Name    : Mouse detection script
# Version : 0.4
# Author  : Sergei Mozhaisky (technix@frenzy.org.ua)
#

. gettext.sh
export TEXTDOMAIN="frenzytools"
export TEXTDOMAINDIR="/usr/local/share/locale"

msg_detect=$(gettext "Detecting mouse...")
msg_running=$(gettext "moused is already running.")
msg_notfound=$(gettext "NOT FOUND")
msg_found=$(gettext "found at")
msg_started=$(gettext "mouse daemon started.")

# -----------------

probe_mouse ()
{
device=$1
FOUND_MOUSE=1
mouseprobe=`moused -i all -p ${device} 2>&1`
if [ -n "`printf "${mouseprobe}" | sed "s/\n//g" | grep ":"`" ]; then
FOUND_MOUSE=0
fi
}

start_mouse ()
{
device=$1
printf " ${msg_found} $device, "
start_moused=`moused -3 -r high -t auto -p ${device} > /dev/null 2>&1`
printf "${msg_started} \n"
start_vidcontrol=`vidcontrol -m on > /dev/null 2>&1`
}

# -----------------------------

printf "$msg_detect"

FOUND_MOUSE=1
#MOUSED_PID=`cat /var/run/moused.pid`
MOUSED_RUNNING=`ps ax | grep moused | grep -v grep`
if [ -n "${MOUSED_RUNNING}" ]; then
printf " $msg_running \n"
exit 0
fi

for i in /dev/psm0 /dev/cuad0 /dev/cuad1 /dev/cuad2 /dev/cuad3 /dev/cuaa0 /dev/cuaa1 /dev/cuaa2 /dev/cuaa3
do
probe_mouse $i
if [ ${FOUND_MOUSE} -eq 1 ]; then
start_mouse $i
exit 0
fi
done
printf " $msg_notfound \n"
exit 0
