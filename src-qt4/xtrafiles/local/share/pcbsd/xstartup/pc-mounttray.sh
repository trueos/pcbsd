#!/bin/sh

if [ "`id -u`" = "0" ] ; then return ; fi

(sleep 20 ; pc-mounttray ) &

