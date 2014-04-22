#!/bin/sh

ESDLDIR=`find ${PBI_PROGDIRPATH}/lib/erlang/lib -type d -maxdepth 1 -name "esdl*" -exec basename {} \;`

if [ "${ESDLDIR}" = "" ]
then
  echo "Can not find Erlang SDL library esdl."
  exit 1
fi

sed -e "s,esdl,${ESDLDIR}," -e "s,-smp,-noinput -smp," ${PBI_PROGDIRPATH}/bin/wings > /tmp/wings
mv /tmp/wings ${PBI_PROGDIRPATH}/bin/wings
chmod 555 ${PBI_PROGDIRPATH}/bin/wings
