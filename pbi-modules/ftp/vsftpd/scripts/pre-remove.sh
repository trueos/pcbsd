#!/bin/sh
killall vsftpd
if [ "$?" = "0" ]
then
  PID=$!
  rm -Rf /usr/home/anon_ftp

  kill -9 $PID
fi


pw user del -n ftp
rm -Rf /usr/home/ftp
cat /etc/rc.conf | grep -v "vsftpd_enable=" > /tmp/.rc.conf
mv /tmp/.rc.conf /etc/rc.conf

