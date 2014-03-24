#!/bin/sh

if [ -z "$1" ] ; then
   LOCALBASE=/usr/local
else
   LOCALBASE="$1"
fi

# Install manpages
mkdir -p ${LOCALBASE}/man/man8 >/dev/null 2>/dev/null
for i in `ls man8/`
do
  if [ -e "${LOCALBASE}/man/man8/${i}.gz" ] ; then
    rm ${LOCALBASE}/man/man8/${i}.gz >/dev/null 2>/dev/null
  fi
  gzip -c man8/${i} > ${LOCALBASE}/man/man8/${i}.gz
done

# Install the executable
if [ ! -d "${LOCALBASE}/bin" ] ; then
  mkdir ${LOCALBASE}/bin
fi
cp pc-systemflag ${LOCALBASE}/bin/pc-systemflag
chown root:wheel ${LOCALBASE}/bin/pc-systemflag
chmod 755 ${LOCALBASE}/bin/pc-systemflag
