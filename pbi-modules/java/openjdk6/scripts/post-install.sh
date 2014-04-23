#!/bin/sh

cd /home
for i in `ls`
do
  if [ -e "${i}/.mozilla/plugins" ]
  then
    ln -fs ${PBI_PROGDIRPATH}/lib/IcedTeaPlugin.so /home/${i}/.mozilla/plugins/IcedTeaPlugin.so
    chown ${i}:${i} /home/${i}/.mozilla/plugins/IcedTeaPlugin.so
  fi
done
