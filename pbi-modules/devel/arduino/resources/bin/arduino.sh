#!/bin/sh

#Setup some internal script variables
PROGDIR=/usr/local   #Base PBI directory (/usr/pbi/myapplication-<arch>)
APPBINARY=arduino   #application binary

#Set the java home path properly
cd ${PROGDIR}
JDKDIR=`ls | grep openjdk`
export JAVA_HOME=${PROGDIR}/${JDKDIR}
echo "JAVA_HOME set to: ${JAVA_HOME}"
#Now start the main application
${APPBINARY} $@
