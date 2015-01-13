#!/bin/sh

# Check if we need to start the system updater service
test="`pbreg get /PC-BSD/SystemUpdater/runAtStartup`"
if [ "$test" = "true" ]; then
  (sleep 62; /usr/local/bin/pc-systemupdatertray)&
fi

