#!/bin/sh
# pbi-manager installation script

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
cp pbi-manager ${LB}/sbin/pbi_info
ln -f ${LB}/sbin/pbi_info ${LB}/sbin/pbi_add
ln -f ${LB}/sbin/pbi_info ${LB}/sbin/pbi_delete
ln -f ${LB}/sbin/pbi_info ${LB}/sbin/pbi_icon
ln -f ${LB}/sbin/pbi_info ${LB}/sbin/pbi_makeindex
ln -f ${LB}/sbin/pbi_info ${LB}/sbin/pbi_updateindex
ln -f ${LB}/sbin/pbi_info ${LB}/sbin/app
ln -f ${LB}/sbin/pbi_info ${LB}/sbin/pbi

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

if [ -d "${LB}/share/pbi-manager" ] ; then 
  rm -rf "${LB}/share/pbi-manager"
fi

mkdir -p ${LB}/share/pbi-manager/ >/dev/null 2>/dev/null

# Copy the icon
mkdir -p ${LB}/share/pbi-manager/icons >/dev/null 2>/dev/null
cp ${DIR}/icons/default.png ${LB}/share/pbi-manager/icons
cp ${DIR}/icons/patch.png ${LB}/share/pbi-manager/icons

# Copy the default repo key / config
cp ${DIR}/repo/repokey.ssl ${LB}/share/pbi-manager/repokey.ssl
cp ${DIR}/repo/pkgng-repo ${LB}/share/pbi-manager/pkgng-repo

# Tickle the info command to setup default dirs
${LB}/sbin/pbi_add >/dev/null 2>/dev/null

cp ${DIR}/etc/pbi.conf ${LB}/etc/pbi.conf
