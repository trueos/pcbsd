#!/bin/sh

DEFAULT="/usr/local"

if [ -z "$1" ] ; then
        LB="${DEFAULT}"
else
        LB="${1}"
fi

# Copy files to system
tar cvf - -C local . 2>/dev/null | tar xvf - -C ${LB}
if [ $? -ne 0 ] ; then
  exit 1
fi

# Make sure each file is owned by root
cd local
for i in `find .`
do
   chown root:wheel ${LB}/${i}
done
cd ..

exit 0
