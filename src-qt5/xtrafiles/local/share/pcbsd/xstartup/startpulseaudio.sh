#!/bin/sh
# Check if we need to start pulseaudio on this login

if [ "`id -u`" = "0" ] ; then return ; fi

# Do we have pulseaudio?
which pulseaudio >/dev/null 2>/dev/null
if [ $? -ne 0 ]; then exit 0; fi

# Check if we need to start it
pulseaudio --check >/dev/null 2>/dev/null
if [ $? -ne 0 ] ; then
   # Start pulseaudio
   pulseaudio --start >/dev/null 2>/dev/null
fi

# Get the default unit
dU=`sysctl hw.snd.default_unit | awk '{print $2}'`

# Set the default sink to match default audio device
pactl set-default-sink $dU >/dev/null 2>/dev/null
