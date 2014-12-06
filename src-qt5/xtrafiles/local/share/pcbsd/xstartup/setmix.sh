#!/bin/sh

MIXOUT="`mixer -s`"

# If ogain is present, make sure its turned up
echo $MIXOUT | grep "ogain " >/dev/null 2>/dev/null
if [ "$?" = "0" ]
then
 mixer ogain 100:100
fi
