#!/bin/sh
# PC-SYSCONFIG install script

DEFAULT="/usr/local"

if [ -z "$1" ] ; then
        LB="${DEFAULT}"
else
        LB="${1}"
fi

DESTDIR=${LB} ; export DESTDIR
QTDESTDIR=`echo $LB | sed 's|/usr/local||g'`

cd daemon
if [ $? -ne 0 ] ; then exit 1 ; fi
/usr/local/lib/qt5/bin/qmake daemon.pro
if [ $? -ne 0 ] ; then exit 1 ; fi
make
if [ $? -ne 0 ] ; then exit 1 ; fi
make install INSTALL_ROOT=${QTDESTDIR}
if [ $? -ne 0 ] ; then exit 1 ; fi

cd ../client
if [ $? -ne 0 ] ; then exit 1 ; fi
/usr/local/lib/qt5/bin/qmake client.pro
if [ $? -ne 0 ] ; then exit 1 ; fi
make
if [ $? -ne 0 ] ; then exit 1 ; fi
make install INSTALL_ROOT=${QTDESTDIR}
if [ $? -ne 0 ] ; then exit 1 ; fi

if [ ! -d "${LB}/etc/rc.d" ] ; then
   mkdir -p ${LB}/etc/rc.d
fi

cd ../
if [ $? -ne 0 ] ; then exit 1 ; fi
cp rc.d/pc-sysconfig ${LB}/etc/rc.d/pc-sysconfig
if [ $? -ne 0 ] ; then exit 1 ; fi
chmod 755 ${LB}/etc/rc.d/pc-sysconfig
if [ $? -ne 0 ] ; then exit 1 ; fi

exit 0
