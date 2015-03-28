#!/bin/sh
# Start life-preserver with login

if [ "`id -u`" = "0" ] ; then return ; fi

life-preserver-tray
