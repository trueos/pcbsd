#!/bin/sh

echo "MSG: Configuring"
echo "apache22_enable=\"YES\"" >> /etc/rc.conf
echo "accf_http_load=\"YES\"" >> /boot/loader.conf

echo "MSG: Starting apache service"
${PBI_RCDIR}/apache22 start

echo "Apache DocumentRoot: ${PBI_PROGDIR}/www/apache22/data"

echo "MSG: Finished!"

