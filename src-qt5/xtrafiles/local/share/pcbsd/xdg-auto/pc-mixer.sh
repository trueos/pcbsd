#!/bin/sh

if [ "`id -u`" = "0" ]
then return ; 
fi

#Startup the mixer tray
pc-mixer

