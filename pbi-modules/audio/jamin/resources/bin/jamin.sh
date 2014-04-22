#!/bin/sh

PROGDIR=/usr/local

ps -auwx | grep "jackd" | grep -v grep >/dev/null 2>/dev/null
if [ "$?" != "0" ]
then
   # Start jack
   ( jackd -r -d oss ) &
   sleep 5
fi

#Properly set the LADSPA plugins path
export LADSPA_PATH=${PROGDIR}/lib/ladspa
# Start calf now
jamin "$@"

killall -9 jackd
