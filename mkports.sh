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
   echo "Usage: ./mkports.sh <outdir>"
   exit 1
fi

if [ ! -d "${1}" ] ; then
   echo "Invalid directory: $1"
   exit 1
fi

portsdir="${1}"
distdir="${1}/distfiles"
if [ ! -d "$portsdir" ] ; then
  mkdir ${portsdir}
fi
if [ ! -d "$portsdir/sysutils" ] ; then
  mkdir ${portsdir}/sysutils
fi
if [ ! -d "$distdir" ] ; then
  mkdir ${distdir}
fi

cliREV=`get_last_rev_git "./src-sh"`
guiREV=`get_last_rev_git "./src-qt4"`

# Make the dist files
rm ${distdir}/pcbsd-utils*.tar.bz2 2>/dev/null
echo "Creating pcbsd-utils dist file for version: $cliREV"
tar cvjf ${distdir}/pcbsd-utils-${cliREV}.tar.bz2 src-sh 2>/dev/null
echo "Creating pcbsd-utils-qt4 dist file for version: $guiREV"
tar cvjf ${distdir}/pcbsd-utils-qt4-${guiREV}.tar.bz2 src-qt4 2>/dev/null

# Copy ports files
rm -rf ${portsdir}/sysutils/pcbsd-utils 2>/dev/null
rm -rf ${portsdir}/sysutils/pcbsd-utils-qt4 2>/dev/null
cp -r src-sh/port-files ${portsdir}/sysutils/pcbsd-utils
cp -r src-qt4/port-files ${portsdir}/sysutils/pcbsd-utils-qt4

# Set the version numbers
sed -i '' "s|CHGVERSION|${cliREV}|g" ${portsdir}/sysutils/pcbsd-utils/Makefile
sed -i '' "s|CHGVERSION|${guiREV}|g" ${portsdir}/sysutils/pcbsd-utils-qt4/Makefile

# Set the mirror to use
sed -i '' "s|http://www.pcbsd.org/~kris/software/|${DURL}|g" ${portsdir}/sysutils/pcbsd-utils/Makefile
sed -i '' "s|http://www.pcbsd.org/~kris/software/|${DURL}|g" ${portsdir}/sysutils/pcbsd-utils-qt4/Makefile

# Create the makesums / distinfo file
cd ${distdir}
sha256 pcbsd-utils-${cliREV}.tar.bz2 > ${portsdir}/sysutils/pcbsd-utils/distinfo
echo "SIZE (pcbsd-utils-${cliREV}.tar.bz2) = `stat -f \"%z\" pcbsd-utils-${cliREV}.tar.bz2`" >> ${portsdir}/sysutils/pcbsd-utils/distinfo
sha256 pcbsd-utils-qt4-${guiREV}.tar.bz2 > ${portsdir}/sysutils/pcbsd-utils-qt4/distinfo
echo "SIZE (pcbsd-utils-qt4-${guiREV}.tar.bz2) = `stat -f \"%z\" pcbsd-utils-qt4-${guiREV}.tar.bz2`" >> ${portsdir}/sysutils/pcbsd-utils-qt4/distinfo
