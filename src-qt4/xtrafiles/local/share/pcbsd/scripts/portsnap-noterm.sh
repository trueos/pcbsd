#!/bin/sh
# Script to run portsnap and remove terminal test
# Allows us to run via GUI

cat /usr/sbin/portsnap | sed 's|! -t 0|-z '1'|g' | /bin/sh -s $@
