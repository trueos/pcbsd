#!/bin/sh
# Starts up the warden service
######################################################################

# Check if we have a template to "prime"
prime_template()
{
   if [ ! -d "/usr/local/tmp/warden-dist" ] ; then return; fi
   DEFTEMPLATE="`uname -r | cut -d '-' -f 1-2`-${ARCH}"
   if [ -d "$JDIR/.warden-template-$DEFTEMPLATE" ] ; then return; fi

   echo "Creating default jail template...";
   FLAGS="-arch $ARCH -nick $DEFTEMPLATE"
   FLAGS="-trueos `uname -r | cut -d '-' -f 1-2` $FLAGS" ; export FLAGS

   WARDENPRIME="TRUE" ; export WARDENPRIME
   (warden template create ${FLAGS} >/tmp/warden_prime_template 2>/tmp/warden_prime_template) &

   unset WARDENPRIME
}

PATH="${PATH}:/usr/local/bin:/usr/local/sbin"
export PATH

# Source our functions
PROGDIR="/usr/local/share/warden"

if [ -z "${PROGDIR}" ]
then
   echo "PROGDIR unset! Is The Warden installed properly?"
   exit 155
fi

# Source our variables
. ${PROGDIR}/scripts/backend/functions.sh

echo "Starting the Warden"

prime_template

# If no jails we can exit
if [ ! -d "${JDIR}" ] ; then exit 0 ; fi

cd ${JDIR}
for i in `ls -d .*.meta 2>/dev/null`
do
  if [ -e "${i}/id" -a -e "${i}/autostart" ] ; then
   JAILNAME=`echo ${i}|sed 's|.meta$||'|sed 's|^.||'`
   echo -e "Starting Jail (${JAILNAME})...\c"
   warden start "${JAILNAME}" >/dev/null 2>/dev/null
   echo "Done"
  fi
done
