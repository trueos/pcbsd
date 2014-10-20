#!/bin/sh
#Quick script to manually fetch/build the PBI index on a system:
# NOTE: Run this script as root from the "pbi-modules" directory in the PC-BSD source tree.
# This will need to be updated occasionally

if [ `id -u` != "0" ] ; then
   echo "Run me as root!"
   exit 1
fi

if [ ! -e "AppCafe-index" ] ; then
   echo "Must be run from modules top-level dir..."
   exit 1
fi

echo "This can take 10+ minutes, lots of pkg data to parse..."
sleep 1

# Run pbi_makeindex now
pbi_makeindex
if [ $? -ne 0 ] ; then
   echo "pbi_makeindex failed!"
   exit 1
fi

#Clean the old index files (comment/uncomment as necessary)
rm -rf /var/db/pbi/index

# Create the new index dir
mkdir -p /var/db/pbi/index

# Copy over the contents of the pbi-modules directory
echo "Moving index data to local system..."
tar xvf PBI-INDEX.txz -C /var/db/pbi/index #2>/dev/null >/dev/null

# Cleanup
rm PBI-INDEX.txz
