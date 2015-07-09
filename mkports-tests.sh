#!/bin/sh
# Script to run test builds of all PC-BSD ports

TEST_PORTS=""

if [ -z "$1" ] ; then
   echo "Usage: ./mkport-tests.sh <portstree>"
   exit 1
fi

if [ ! -d "${1}/Mk" ] ; then
   echo "Invalid directory: $1"
   exit 1
fi

portsdir="${1}"
if [ -z "$2" ] ; then
  distdir="${1}/distfiles"
else
  distdir="${2}"
fi

# Start by copying all the ports over
./mkports.sh ${1} ${2}
if [ $? -ne 0 ] ; then exit 1; fi

# Get this jail version
export UNAME_r="`freebsd-version`"

if [ ! -d "/tmp/.pcbsd-tests" ] ; then
  mkdir /tmp/.pcbsd-tests
fi

# Now start building the various ports
while read portline
do
  port=`echo $portline | awk '{print $2}'`
  tverfile="/tmp/.pcbsd-tests/`echo $port | sed 's|/|_|g'`"

  cd $portsdir/$port

  if [ -e "$tverfile" ] ; then
     # If this file exists, we did a previous build of this port
     nVer=`make DISTVERSION`
     oVer=`cat $tverfile`
     if [ "$nVer" = "$oVer" ] ; then
       echo "No changes to port: $port"
       continue
     fi
  fi

  echo "Building port: $port"
  if [ $? -ne 0 ] ; then exit 1; fi

  make clean

  make BATCH=yes
  if [ $? -ne 0 ] ; then exit 1; fi
  
  make stage
  if [ $? -ne 0 ] ; then exit 1; fi

  make check-plist
  if [ $? -ne 0 ] ; then exit 1; fi

  make install
  if [ $? -ne 0 ] ; then exit 1; fi

  make package
  if [ $? -ne 0 ] ; then exit 1; fi

  make clean
  if [ $? -ne 0 ] ; then exit 1; fi

  # Save the version number so we can skip on next run if no changes
  make -V DISTVERSION > $tverfile
done < mkports-list

exit 0
