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

cd qtapp
if [ $? -ne 0 ] ; then exit 1 ; fi
/usr/local/lib/qt5/bin/qmake daemon.pro
if [ $? -ne 0 ] ; then exit 1 ; fi
make
if [ $? -ne 0 ] ; then exit 1 ; fi
make install INSTALL_ROOT=${QTDESTDIR}
if [ $? -ne 0 ] ; then exit 1 ; fi

exit 0
