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
DIR=`realpath $DIR`
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
ln -f ${LB}/sbin/pbi_create ${LB}/sbin/app
ln -f ${LB}/sbin/pbi_create ${LB}/sbin/pbi

# Set the new PREFIX / LOCALBASE
if [ "$LB" != /usr/local ] ; then
  sed -i '' "s|PROGBASE=/usr/local|PROGBASE=$LB|g" ${LB}/sbin/pbi_create
fi

# Install manpages
mkdir -p ${LB}/man/man1 >/dev/null 2>/dev/null
for i in `ls man1/`
do
  rm ${LB}/man/man1/${i}.gz >/dev/null 2>/dev/null
  gzip -c man1/${i} > ${LB}/man/man1/${i}.gz
done

mkdir -p ${LB}/man/man5 >/dev/null 2>/dev/null
for i in `ls man5/`
do
  rm ${LB}/man/man5/${i}.gz >/dev/null 2>/dev/null
  gzip -c man5/${i} > ${LB}/man/man5/${i}.gz
done


mkdir -p ${LB}/man/man8 >/dev/null 2>/dev/null
for i in `ls man8/`
do
  rm ${LB}/man/man8/${i}.gz >/dev/null 2>/dev/null
  gzip -c man8/${i} > ${LB}/man/man8/${i}.gz
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
#if [ ! -d "/var/db/pbi/keys" ] ; then
#	pbi_addrepo ${DIR}/repo/pcbsd.rpo
#fi

# Copy the default PC-BSD repo file
cp ${DIR}/repo/pcbsd.rpo ${LB}/share/pbi-manager/pcbsd.rpo

# Tickle the info command to setup default dirs
${LB}/sbin/pbi_info >/dev/null 2>/dev/null

# Copy pbid / pbi.conf
cp ${DIR}/rc.d/pbid ${LB}/etc/rc.d/pbid
cp ${DIR}/etc/pbi.conf ${LB}/etc/pbi.conf

# Create the wrapper binary
cd ${DIR}/wrapper
if [ `uname -m` = "amd64" ] ; then
  # Build 32bit wrapper
  echo "Building i386 wrapper..."
  rm pbiwrapper >/dev/null 2>/dev/null
  make clean
  make DEFINES="-mtune=i386 -march=i386 -m32"
  make install
  chown root:wheel pbiwrapper 
  chmod 644 pbiwrapper
  mv pbiwrapper ${LB}/share/pbi-manager/.pbiwrapper-i386
  if [ "${LB}" = "/usr/local" ] ; then
     install -o root -g wheel -m 755 ${LB}/share/pbi-manager/.pbiwrapper-i386 /usr/pbi/.pbi-wrapper-i386
  fi
else
  touch ${LB}/share/pbi-manager/.pbiwrapper-amd64
fi

# Build system arch wrapper
echo "Building `uname -m` wrapper..."
DEFINES=""
export DEFINES
rm pbiwrapper >/dev/null 2>/dev/null
make clean
make DEFINES=""
make install DEFINES=""
chown root:wheel pbiwrapper 
chmod 644 pbiwrapper
mv pbiwrapper ${LB}/share/pbi-manager/.pbiwrapper-`uname -m`
if [ "${LB}" = "/usr/local" ] ; then
   install -o root -g wheel -m 755 ${LB}/share/pbi-manager/.pbiwrapper-`uname -m` /usr/pbi/.pbi-wrapper-`uname -m`
fi

# Install the pbime wrapper
cd ${DIR}/pbime && make
install -o root -g wheel -m 755 pbime ${LB}/share/pbi-manager/.pbime
install -o root -g wheel -m 4751 pbild ${LB}/share/pbi-manager/.pbild
install -o root -g wheel -m 755 pbirun ${LB}/share/pbi-manager/.pbirun
install -o root -g wheel -m 755 pbirun32 ${LB}/share/pbi-manager/.pbirun32
install -o root -g wheel -m 755 pbiinit ${LB}/share/pbi-manager/.pbiinit
install -o root -g wheel -m 755 ldconfig ${LB}/share/pbi-manager/.ldconfig
install -o root -g wheel -m 755 pbisyscmd ${LB}/share/pbi-manager/.pbisyscmd
install -o root -g wheel -m 755 openwith ${LB}/share/pbi-manager/openwith
if [ "${LB}" = "/usr/local" ] ; then
  install -o root -g wheel -m 755 pbime /usr/pbi/.pbime
  install -o root -g wheel -m 4751 pbild /usr/pbi/.pbild
  install -o root -g wheel -m 755 pbirun /usr/pbi/.pbirun
  install -o root -g wheel -m 755 pbirun32 /usr/pbi/.pbirun32
  install -o root -g wheel -m 755 pbiinit /usr/pbi/.pbiinit
  install -o root -g wheel -m 755 ldconfig /usr/pbi/.ldconfig
  install -o root -g wheel -m 755 pbisyscmd /usr/pbi/.pbisyscmd
  install -o root -g wheel -m 755 openwith /usr/bin/openwith
fi

# Install the pbi_preload wrapper library
cd ${DIR}/pbi_preload && make 
install -o root -g wheel -m 755 pbi_preload.so ${LB}/share/pbi-manager/.pbi_preload.so
install -o root -g wheel -m 755 pbi_preload32.so ${LB}/share/pbi-manager/.pbi_preload32.so
if [ "${LB}" = "/usr/local" ] ; then
  install -o root -g wheel -m 755 pbi_preload.so /usr/pbi/.pbi_preload.so
  install -o root -g wheel -m 755 pbi_preload32.so /usr/pbi/.pbi_preload32.so
fi

# Install the MANPATH conf
if [ ! -d "${LB}/etc/man.d" ] ; then
   mkdir -p "${LB}/etc/man.d"
fi
cp ${DIR}/man.d/pbi.conf ${LB}/etc/man.d/pbi.conf
