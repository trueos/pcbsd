#!/bin/sh
# Script which will set values
#
# Called by AppCafe with:
# $1 = Key which needs updating
# $2 = Value we have set Key to
# $3 = {YES|NO} - YES if this is the last value the user changed
#      (Used to optionally restart services, etc)

KEY="$1"
VALUE="$2"
ATEND="$3"
CFGFILE="/usr/local/etc/CHANGEME"
SERVICENAME="CHANGEME"

# Get value from config file
oldvalue=`grep "^${KEY} = " $CFGFILE 2>/dev/null | head -n 1 | sed "s|${KEY} = ||g" | cut -d ' ' -f 1`

# If no old line found, lets add it
if [ -z "$oldvalue" ] ; then
  newline="${KEY} = ${VALUE}"

  # Add the line
  echo "$newline" >> $CFGFILE
else
  # Get the complete old line
  oldline=`grep "^${KEY} = " $CFGFILE 2>/dev/null | head -n 1`

  # Create the new line
  newline=`echo $oldline | sed "s|$oldvalue|$VALUE|g"`

  # Let sed work its magic
  sed -i '' "s/^${oldline}.*/${newline}/" $CFGFILE
fi

# If we are done changing values
if [ "$ATEND" = "YES" ] ; then
  # See if we need to restart bitlbee
  grep -q '^CHANGEME_enable="YES"' /etc/rc.conf
  if [ $? -eq 0 ] ; then
     # Restart bitlbee
     service ${SERVICENAME} restart
  fi
fi
