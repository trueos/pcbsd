#!/bin/sh

cd ${PBI_PROGDIRPATH}


mkdir -p /var/db/ntop
chmod 777 /var/db/ntop

if [ ! -z "${DISPLAY}" ]
then
else
  echo "ntop is installed! ntop is a command-line application, to use run 'ntop' from a shell session"
fi
