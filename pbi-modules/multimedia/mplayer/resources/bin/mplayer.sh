#!/bin/sh

findLocale()
{	if [ "`echo $LANG | grep :`" != "" ]
	then
		LANG="`echo $LANG | cut -d ":" -f1`"
	fi
	LOCALELIST="`cd /usr/local/share/locale && ls | grep -v UTF-8 | grep $LANG | grep _`"
	echo "`echo $LOCALELIST | cut -d \" \" -f1`.UTF-8"
}
#echo "LANG:$LANG"
LANG="`findLocale`"
#echo "`locale`"
cd 
gmplayer $@
