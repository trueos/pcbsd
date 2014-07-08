#!/bin/sh

if [ "`id -u`" = "0" ] ; then return ; fi

(sleep 30 ; pc-mounttray ) &

