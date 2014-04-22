#!/bin/sh

#Create the initial jasperserver database from the work dir 
#(work dir gets removed later during packaging, so need to do it now)
echo "Creating initial JasperServer database"
cd /usr/workdirprefix/usr/ports/databases/jasperserver/work/jasperreports-server-*-src/jasperserver/buildomatic
./js-ant create-js-db
echo "Done creating initial JasperServer database"
