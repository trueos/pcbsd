#!/bin/sh
# personacrypt installation script

DEFAULT="/usr/local"

if [ -z "$1" ] ; then 
	LB="${DEFAULT}" 
else
	LB="${1}" 
fi

DIR=`dirname $0`
DIR=`realpath $DIR`
cd ${DIR}

# Install the app
mkdir ${LB}/bin >/dev/null 2>/dev/null
cp personacrypt ${LB}/bin/
chmod 755 ${LB}/bin/personacrypt
