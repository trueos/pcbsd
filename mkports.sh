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
   rev=`git log -n 1 --date=raw . | grep 'Date:' | awk '{print $2}'`
   cd $oPWD
   if [ -n "$rev" ] ; then
     echo "$rev"
     return 0
   else
     rev=`git log -n 1 --date=raw . | grep 'Date:' | awk '{print $2}'`
     echo $rev
     return 0
   fi
   return 1
}

massage_subdir() {
  cd $1
  if [ $? -ne 0 ] ; then
     echo "SKIPPING $i"
     continue
  fi

  echo "# \$FreeBSD\$
#

$comment
" > Makefile.tmp

  for d in `ls`
  do
    if [ "$d" = ".." ]; then continue ; fi
    if [ "$d" = "." ]; then continue ; fi
    if [ "$d" = "Makefile" ]; then continue ; fi
    if [ ! -f "$d/Makefile" ]; then continue ; fi
    echo "    SUBDIR += $d" >> Makefile.tmp
  done
  echo "" >> Makefile.tmp
  echo ".include <bsd.port.subdir.mk>" >> Makefile.tmp
  mv Makefile.tmp Makefile

}

if [ -z "$1" ] ; then
   echo "Usage: ./mkports.sh <portstree> <distfiles>"
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
if [ ! -d "$distdir" ] ; then
  mkdir -p ${distdir}
fi

ODIR=`pwd`

echo "Sanity checking the repo..."
OBJS=`find . | grep '\.o$'`
if [ -n "$OBJS" ] ; then
   echo "Found the following .o files, remove them first!"
   echo $OBJS
   exit 1
fi

# Get this jail version
export UNAME_r="`freebsd-version`"

# Read the list of ports and build them now
while read pline
do

  cd $ODIR
  ldir=`echo $pline | awk '{print $1}'`

  # Check for sub-dir
  echo "$ldir" | grep -q '/'
  if [ $? -eq 0 ] ; then
    lsubdir=`echo $ldir | cut -d '/' -f 1`
    ltopdir=`echo $ldir | cut -d '/' -f 2`
    tOps="-C $lsubdir $ltopdir"
  else
    unset lsubdir ltopdir
    tOps="$ldir"
  fi

  tdir=`echo $pline | awk '{print $2}'`
  tcat=`echo $tdir | cut -d '/' -f 1`
  dfile=`echo $pline | awk '{print $3}'`

  # Get git revision number
  REV=`get_last_rev_git "./$ldir"`

  port=`echo $pline | awk '{print $2}'`
  tverfile="/tmp/.pcbsd-tests/`echo $port | sed 's|/|_|g'`"
  if [ -e "$tverfile" -a -n "$PCBSD_MKTESTS" ] ; then
    # If this file exists, we did a previous build of this port
    oVer=`cat $tverfile`
    echo "$port - $REV - $oVer -"
    if [ "$REV" = "$oVer" ] ; then
      echo "No changes to port: $port"
      continue
    fi
  fi

  # Make the dist files
  rm ${distdir}/${dfile}-* 2>/dev/null
  echo "Creating $tdir dist file for version: $REV"
  tar cvJf ${distdir}/${dfile}-${REV}.tar.xz --exclude .git $tOps 2>/dev/null >/dev/null
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
  sha256 $dfile-${REV}.tar.xz > ${portsdir}/${tdir}/distinfo
  echo "SIZE ($dfile-${REV}.tar.xz) = `stat -f \"%z\" $dfile-${REV}.tar.xz`" >> ${portsdir}/$tdir/distinfo

  # Now make sure subdir Makefile is correct
  massage_subdir "${portsdir}/$tcat"

done < mkports-list

cd $ODIR

# Get the current timestamp
TIMESTAMP="`date +%s`"

# Add the files from build-files/ports-overlay
for i in `find build-files/ports-overlay/ | grep Makefile`
do

  portMake=`echo $i | sed 's|build-files/ports-overlay/||g'`
  port=`echo $portMake | sed 's|/Makefile||g'`
  tcat=`echo $port | cut -d '/' -f 1`

  echo "Adding port: $port"

  # Copy the port now
  if [ -e "${portsdir}/${port}" ] ; then
     rm -rf ${portsdir}/${port}
  fi
  cp -r build-files/ports-overlay/${port} ${portsdir}/${port}

  # Set the version number in these ports
  mREV=`get_last_rev_git "./build-files/ports-overlay/$port"`
  sed -i '' "s|CHGVERSION|$mREV|g" ${portsdir}/${portMake}
  sed -i '' "s|%TIMESTAMP%|$TIMESTAMP|g" ${portsdir}/${portMake}

  if [ "$port" = "misc/pcbsd-i18n-qt5" ] ; then
     cd ${distdir}
     fetch -o pcbsd-i18n-qt5-${TIMESTAMP}.tar.xz https://github.com/pcbsd/pcbsd-i18n/raw/master/dist/pcbsd-i18n.txz
     sha256 pcbsd-i18n-qt5-${TIMESTAMP}.tar.xz > ${portsdir}/${port}/distinfo
     echo "SIZE (pcbsd-i18n-qt5-${TIMESTAMP}.tar.xz) = `stat -f \"%z\" pcbsd-i18n-qt5-${TIMESTAMP}.tar.xz`" >> ${portsdir}/$port/distinfo
  fi

  # Now make sure subdir Makefile is correct
  massage_subdir "${portsdir}/$tcat"
  cd $ODIR
done
