#!/bin/sh
# Check if we need to show the welcome application

if [ "`id -u`" = "0" ] ; then return ; fi

if [ ! -f "${HOME}/.config/hide-welcome" ] ; then
  pc-welcome
fi
