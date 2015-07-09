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

cd `realpath $0`

# Start by copying all the ports over
./mkports.sh ${1} ${2}
if [ $? -ne 0 ] ; then exit 1; fi

# Now start building the various ports
while read portline
do
  port=`echo $portline | awk '{print $2}'`

  PORTREMOVE=

  echo "Building port: $port"
  cd $port
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
done < mkports-list

exit 0
