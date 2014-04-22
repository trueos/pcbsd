#!/bin/sh

#Setup some internal script variables
PROGDIR=`pwd | cut -d / -f 1-4`   #Base PBI directory (/usr/pbi/myapplication-<arch>)
APPBINARY=bin/netbeans   #application binary

#Now start the main application with additional flags
${PROGDIR}/${APPBINARY} -J-Dnetbeans.subversion.commit.deepStatusRefresh=false $@
