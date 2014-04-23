#!/bin/sh

for i in `ls ${PBI_PROGDIRPATH}/bin/*.pl`
do
  sed -i.bak "s,/usr/bin/perl,${PBI_PROGDIRPATH}/bin/perl,g" $i
done
