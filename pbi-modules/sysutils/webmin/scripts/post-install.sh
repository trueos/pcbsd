#!/bin/sh

echo "TOTALSTEPS: 4"
echo "SETSETPS: 1"

cd ${PBI_PROGDIRPATH}

ln -s ${PBI_PROGDIRPATH}/etc/rc.d/webmin ${PBI_RCDIR}/webmin

chmod -Rf 755 ${PBI_PROGDIRPATH}/services/webmin/*

echo "SETSETPS: 2"
echo "MSG: Running Webmin setup..."
echo -e "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" > /tmp/.webmin_tmp.tmp
${PBI_PROGDIRPATH}/lib/webmin/setup.sh < /tmp/.webmin_tmp.tmp
rm -f /tmp/.webmin_tmp.tmp

echo "SETSETPS: 3"
echo "MSG: Starting webmin..."
echo "" >> /etc/rc.conf
echo "webmin_enable=\"YES\"" >> /etc/rc.conf
${PBI_RCDIR}/webmin start

echo "MSG: Finished!"
echo "SETSETPS: 4"
