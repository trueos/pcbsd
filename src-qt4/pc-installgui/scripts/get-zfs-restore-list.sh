#!/bin/sh
# Script for Life-Preserver which checks a remote box and prints a list of 
# available restore points
###########################################################################

SSHUSER=$1
SSHHOST=$2
SSHPORT=$3
SSHKEY=$4

if [ -z "${SSHUSER}" -o -z "${SSHHOST}" -o -z "${SSHPORT}" ]; then
  echo "ERROR: Usage get-zfs-restore-list.sh <user> <host> <port>"
  touch /tmp/.ssh-list-failed
  exit 150
fi

if [ -n "$SSHKEY" ] ; then
   SSHKEY="-i $SSHKEY"
fi

cd /root

ssh ${SSHKEY} -p $SSHPORT -o StrictHostKeyChecking=no -o PasswordAuthentication=no -o NumberOfPasswordPrompts=0 $SSHUSER@$SSHHOST 'ls .lp-props*' 2>/dev/null
exit $?
