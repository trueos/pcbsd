#!/bin/sh
# This script checks if a service is currently running
# Return a "0" if the service is running, or a "1" if it is stopped
ps -x | grep -v "grep" | grep webmin
if [ "$?" = "0" ]
then
  exit 0
else
  exit 1
fi

