#!/bin/sh

PROGDIR=/usr/local

#Make sure the user configuration file exists
if [ ! -f ~/.tpadrc ]; then
  cp ${PROGDIR}/etc/tpad.conf.sample ~/.tpadrc
fi

#Startup the main application
${PROGDIR}/bin/tpad
