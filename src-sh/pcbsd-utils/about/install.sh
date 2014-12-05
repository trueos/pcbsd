#!/bin/sh
# Script to set "about" version number

# Source config file
. ../config.sh

DEFAULT="/usr/local"

if [ -z "$1" ] ; then 
	LB="${DEFAULT}" 
else
	LB="${1}" 
fi

DIR=`dirname $0`
DIR=`realpath $DIR`
cd ${DIR}

if [ ! -d "${LB}/share/pcbsd" ] ; then
  mkdir -p ${LB}/share/pcbsd/
fi

echo "$PCBSDVERSION" > ${LB}/share/pcbsd/version
