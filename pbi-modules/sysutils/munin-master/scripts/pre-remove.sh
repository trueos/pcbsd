#!/bin/sh

if [ -d /var/log/munin ]; then
  echo "Removing directory: /var/log/munin/"
  rm -rf /var/log/munin/
fi