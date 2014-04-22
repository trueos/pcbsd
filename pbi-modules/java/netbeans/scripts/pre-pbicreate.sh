#!/bin/sh

#Make a generic "netbeans" binary that does not use the version in the name
cd ${PBI_PROGDIRPATH}/bin
nb=`ls netbeans-*`
ln -s $nb netbeans
