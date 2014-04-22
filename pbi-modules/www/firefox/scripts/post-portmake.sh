#!/bin/sh

cp ${PBI_PROGDIRPATH}/lib/libevent* ${PBI_PROGDIRPATH}/lib/firefox/

rm -rf ${PBI_PROGDIRPATH}/include
rm -rf ${PBI_PROGDIRPATH}/man
rm -rf ${PBI_PROGDIRPATH}/share/doc
rm -rf ${PBI_PROGDIRPATH}/lib/libxul
