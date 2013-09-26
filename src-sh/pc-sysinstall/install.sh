#!/bin/sh

if [ -z "$1" ] ; then
   LOCALBASE=/usr/local
else
   LOCALBASE="$1"
fi

if [ -d "${LOCALBASE}/share/pc-sysinstall" ] ; then
   rm -rf ${LOCALBASE}/share/pc-sysinstall
fi

mkdir -p ${LOCALBASE}/share/pc-sysinstall

# Copy backend / conf / doc
cp -r backend ${LOCALBASE}/share/pc-sysinstall/
cp -r backend-partmanager ${LOCALBASE}/share/pc-sysinstall/
cp -r backend-query ${LOCALBASE}/share/pc-sysinstall/
cp -r conf ${LOCALBASE}/share/pc-sysinstall/
cp -r doc ${LOCALBASE}/share/pc-sysinstall/
cp -r examples ${LOCALBASE}/share/pc-sysinstall/
chown -R root:wheel ${LOCALBASE}/share/pc-sysinstall

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
if [ ! -d "${LOCALBASE}/sbin" ] ; then
  mkdir ${LOCALBASE}/sbin
fi
cp pc-sysinstall ${LOCALBASE}/sbin/pc-sysinstall
chown root:wheel ${LOCALBASE}/sbin/pc-sysinstall
chmod 755 ${LOCALBASE}/sbin/pc-sysinstall
