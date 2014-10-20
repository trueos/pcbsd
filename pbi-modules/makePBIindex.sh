#!/bin/sh
#Quick script to manually fetch/build the PBI index on a system:
# NOTE: Run this script as root from the "pbi-modules" directory in the PC-BSD source tree.
# This will need to be updated occasionally

#Clean the old index files (comment/uncomment as necessary)
#rm -rf /var/db/pbi/index/*

#Copy over the contents of the pbi-modules directory
cp -Rf * /var/db/pbi/index/.

#Now run pbi_makeindex for each of the PBI modules
for i in `find /var/db/pbi/index | grep pbi.conf`
do 
 cd `dirname ${i}`
 pbi_makeindex
done