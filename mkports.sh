#!/bin/sh
# Helper script which will create the port / distfiles
# from a checked out git repo

# Set the distfile URL we will fetch from
DURL="http://www.pcbsd.org/~kris/software/"

get_last_rev_git()
{
   oPWD=`pwd`
   cd "${1}"
   rev=0
   rev=`git log -n 1 --date=raw | grep 'Date:' | awk '{print $2}'`
   cd $oPWD
   if [ $rev -ne 0 ] ; then
     echo "$rev"
     return 0
   fi
   return 1
}

if [ -z "$1" ] ; then
   echo "Usage: ./mkports.sh <portstree>"
   exit 1
fi

if [ ! -d "${1}/Mk" ] ; then
   echo "Invalid directory: $1"
   exit 1
fi

portsdir="${1}"
distdir="${1}/distfiles"
if [ ! -d "$distdir" ] ; then
  mkdir ${distdir}
fi

ODIR=`pwd`

# Read the list of ports and build them now
while read pline
do
  cd $ODIR
  ldir=`echo $pline | awk '{print $1}'`
  tdir=`echo $pline | awk '{print $2}'`
  dfile=`echo $pline | awk '{print $3}'`

  REV=`get_last_rev_git "./$ldir"`

  # Make the dist files
  rm ${distdir}/${dfile}-* 2>/dev/null
  echo "Creating $tdir dist file for version: $REV"
  tar cvjf ${distdir}/${dfile}-${REV}.tar.bz2 $ldir 2>/dev/null >/dev/null
  if [ $? -ne 0 ] ; then
     echo "Error creating distfile..."
     exit 1
  fi

  # Copy ports files
  if [ -d "${portsdir}/$tdir" ] ; then
    rm -rf ${portsdir}/$tdir 2>/dev/null
  fi
  cp -r ${ldir}/port-files ${portsdir}/$tdir

  # Set the version numbers
  sed -i '' "s|CHGVERSION|${REV}|g" ${portsdir}/$tdir/Makefile

  # Set the mirror to use
  sed -i '' "s|http://www.pcbsd.org/~kris/software/|${DURL}|g" ${portsdir}/$tdir/Makefile

  # Create the makesums / distinfo file
  cd ${distdir}
  sha256 $dfile-${REV}.tar.bz2 > ${portsdir}/${tdir}/distinfo
  echo "SIZE ($dfile-${REV}.tar.bz2) = `stat -f \"%z\" $dfile-${REV}.tar.bz2`" >> ${portsdir}/$tdir/distinfo

done < mkports-list
