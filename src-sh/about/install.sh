#!/bin/sh
# Script to set "about" version number

VER=`cat ../../src-qt4/config.h | grep '#define PCBSDVERSION' | cut -d '"' -f 2`

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

echo "$VER" > ${LB}/share/pcbsd/version
