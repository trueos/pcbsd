#!/bin/sh
# Script for Life-Preserver which sets up password-less logins for ssh host
###########################################################################

SSHUSER=$1
SSHHOST=$2
SSHPORT=$3

if [ -z "${SSHUSER}" -o -z "${SSHHOST}" ]
then
echo "ERROR: Usage setup-ssh-keys.sh <user> <host>"
  exit 150
fi

cd /root

echo "Preparing to setup SSH key authorization..."
echo "When prompted, enter your password for ${SSHUSER}@${SSHHOST}"

if [ ! -e ".ssh/id_rsa.pub" ]
then
mkdir .ssh >/dev/null 2>/dev/null
  ssh-keygen -q -t rsa -N '' -f .ssh/id_rsa
  sleep 1
fi

if [ ! -e ".ssh/id_rsa.pub" ]
then
echo "ERROR: Failed creating .ssh/id_rsa.pub"
  read tmp
  exit 150
fi

# Get the .pub key
PUBKEY="`cat .ssh/id_rsa.pub`"

echo $PUBKEY | ssh -p $SSHPORT $SSHUSER@$SSHHOST 'mkdir .ssh 2>/dev/null; chmod 700 .ssh ; tee -a .ssh/authorized_keys ; chmod 644 .ssh/authorized_keys'

echo $PUBKEY | ssh -p $SSHPORT $SSHUSER@$SSHHOST 'mkdir .ssh 2>/dev/null; chmod 700 .ssh ; tee -a .ssh/authorized_keys2 ; chmod 644 .ssh/authorized_keys2'

if [ "$?" != "0" ]; then
  touch /tmp/.failedPreserver-${SSHUSER}
fi

echo "Press ENTER to continue"
read tmp
