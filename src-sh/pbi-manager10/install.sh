#!/bin/sh
# pbi-manager installation script

# Check if this is for the right version of FreeBSD
verCheck="`grep '^#define __FreeBSD_version' /usr/include/sys/param.h | awk '{print $3}'`"
if [ $verCheck -lt 1000000 ] ; then
  # This version is for FreeBSD >= 10
  exit 0
fi

DEFAULT="/usr/local"

if [ -z "$1" ] ; then 
	LB="${DEFAULT}" 
else
	LB="${1}" 
fi

DIR=`dirname $0`
cd ${DIR}

# Install the app
mkdir ${LB}/sbin >/dev/null 2>/dev/null
cp pbi-manager ${LB}/sbin/pbi_create
ln -f ${LB}/sbin/pbi_create ${LB}/sbin/pbi_add
ln -f ${LB}/sbin/pbi_create ${LB}/sbin/pbi_addrepo
ln -f ${LB}/sbin/pbi_create ${LB}/sbin/pbi_browser
ln -f ${LB}/sbin/pbi_create ${LB}/sbin/pbi_autobuild
ln -f ${LB}/sbin/pbi_create ${LB}/sbin/pbi_delete
ln -f ${LB}/sbin/pbi_create ${LB}/sbin/pbi_deleterepo
ln -f ${LB}/sbin/pbi_create ${LB}/sbin/pbi_icon
ln -f ${LB}/sbin/pbi_create ${LB}/sbin/pbi_info
ln -f ${LB}/sbin/pbi_create ${LB}/sbin/pbi_indextool
ln -f ${LB}/sbin/pbi_create ${LB}/sbin/pbi_listrepo
ln -f ${LB}/sbin/pbi_create ${LB}/sbin/pbi_makepatch
ln -f ${LB}/sbin/pbi_create ${LB}/sbin/pbi_makeport
ln -f ${LB}/sbin/pbi_create ${LB}/sbin/pbi_makerepo
ln -f ${LB}/sbin/pbi_create ${LB}/sbin/pbi_metatool
ln -f ${LB}/sbin/pbi_create ${LB}/sbin/pbi_patch
ln -f ${LB}/sbin/pbi_create ${LB}/sbin/pbi_update
ln -f ${LB}/sbin/pbi_create ${LB}/sbin/pbi_update_hashdir
ln -f ${LB}/sbin/pbi_create ${LB}/sbin/pbid
ln -f ${LB}/sbin/pbi_create ${LB}/sbin/pbi-crashhandler

# Set the new PREFIX / LOCALBASE
if [ "$LB" != /usr/local ] ; then
  sed -i '' "s|PROGBASE=/usr/local|PROGBASE=$LB|g" ${LB}/sbin/pbi_create
fi

# Install manpages
mkdir -p ${LB}/man/man1 >/dev/null 2>/dev/null
for i in `ls man1/`
do
  rm ${LB}/man/man1/${i}.gz >/dev/null 2>/dev/null
  cp man1/${i} ${LB}/man/man1/${i}
done

mkdir -p ${LB}/man/man5 >/dev/null 2>/dev/null
for i in `ls man5/`
do
  rm ${LB}/man/man5/${i}.gz >/dev/null 2>/dev/null
  cp man5/${i} ${LB}/man/man5/${i}
done


mkdir -p ${LB}/man/man8 >/dev/null 2>/dev/null
for i in `ls man8/`
do
  rm ${LB}/man/man8/${i}.gz >/dev/null 2>/dev/null
  cp man8/${i} ${LB}/man/man8/${i}
done

if [ -d "${LB}/share/pbi-manager" ] ; then rm -rf "${LB}/share/pbi-manager" ; fi

# Copy over the examples
mkdir -p ${LB}/share/pbi-manager/ >/dev/null 2>/dev/null
cp -r ${DIR}/module-examples ${LB}/share/pbi-manager/

# Copy the icon
mkdir -p ${LB}/share/pbi-manager/icons >/dev/null 2>/dev/null
cp ${DIR}/icons/default.png ${LB}/share/pbi-manager/icons
cp ${DIR}/icons/patch.png ${LB}/share/pbi-manager/icons

# If this is a new install, add the PC-BSD master repo
if [ ! -d "/var/db/pbi/keys" ] ; then
	pbi_addrepo ${DIR}/repo/pcbsd.rpo
fi

# Copy the default PC-BSD repo file
cp ${DIR}/repo/pcbsd.rpo ${LB}/share/pbi-manager/pcbsd.rpo

# Tickle the info command to setup default dirs
${LB}/sbin/pbi_info >/dev/null 2>/dev/null

# Copy pbid / pbi.conf
cp ${DIR}/rc.d/pbid ${LB}/etc/rc.d/pbid
cp ${DIR}/etc/pbi.conf ${LB}/etc/pbi.conf

if [ ! -d "/usr/pbi" ] ; then mkdir /usr/pbi ; fi

# Create the wrapper binary
cd ${DIR}/wrapper
if [ `uname -m` = "amd64" ] ; then
  # Build 32bit wrapper
  echo "Building i386 wrapper..."
  rm .pbiwrapper >/dev/null 2>/dev/null
  make clean
  make DEFINES="-mtune=i386 -march=i386 -m32"
  make install
  chown root:wheel /usr/pbi/.pbiwrapper 
  chmod 644 /usr/pbi/.pbiwrapper
  mv /usr/pbi/.pbiwrapper ${LB}/share/pbi-manager/.pbiwrapper-i386
else
  touch ${LB}/share/pbi-manager/.pbiwrapper-i386
fi

# Build system arch wrapper
echo "Building `uname -m` wrapper..."
DEFINES=""
export DEFINES
rm .pbiwrapper >/dev/null 2>/dev/null
make clean
make DEFINES=""
make install DEFINES=""
chown root:wheel /usr/pbi/.pbiwrapper 
chmod 644 /usr/pbi/.pbiwrapper
mv /usr/pbi/.pbiwrapper ${LB}/share/pbi-manager/.pbiwrapper-`uname -m`
