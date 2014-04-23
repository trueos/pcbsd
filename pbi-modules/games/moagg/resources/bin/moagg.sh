#!/bin/sh
#This is a sample wrapper script form layout

PBI_PROGDIR=`pwd | cut -d / -f 1-4`  #base PBI directory (/usr/pbi/my-application-<arch>)
APPBINARY=bin/moagg 

export JAVA_HOME=${PBI_PROGDIR}/`ls ${PBI_PROGDIR} | grep openjdk`
echo "Set JAVA_HOME to: ${JAVA_HOME}"

#Now start the main application
${PBI_PROGDIR}/${APPBINARY} $@
