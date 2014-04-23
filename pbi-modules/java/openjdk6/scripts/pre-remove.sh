#!/bin/sh

rm /usr/local/openjdk6

cd /home
for i in `ls`
do
  if [ -e "${i}/.mozilla/plugins/IcedTeaPlugin.so" ]
  then
    rm /home/${i}/.mozilla/plugins/IcedTeaPlugin.so
  fi
done
