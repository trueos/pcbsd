#!/bin/sh

PROGDIR=$1

cd ~
if [ -e ".purple/prefs.xml" ]
then
   sed '/Pidgin[0-9].[0-9].[0-9]/s//'${PROGDIR}'/' .purple/prefs.xml > .purple/prefs.xml_update
   mv .purple/prefs.xml_update .purple/prefs.xml
fi

!DSPAM:1,4a725e9515166438318951!
