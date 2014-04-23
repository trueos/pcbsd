#!/bin/sh

echo "vsftpd_enable=\"YES\"" >> /etc/rc.conf
ln -s ${PBI_PROGDIRPATH}/vsftpd /usr/local/etc/rc.d/vsftpd

if [ "$?" = "0" ] 
then 
	if [ "$USERS" != "b" ] 
	then
		sed -i.bak 's/anonymous_enable=NO/anonymous_enable=YES/g' ${PBI_PROGDIRPATH}/local/vsftpd.conf
		#vsftpdanon
		pw user add -n ftp -c "FTP User" -m -s /sbin/nologin -w none
		chmod 755 ${PBI_PROGDIRPATH}/anon_ftp
		cd ${PBI_PROGDIRPATH}/anon_ftp
		mkdir ${PBI_PROGDIRPATH}/anon_ftp/Uploads
		mkdir ${PBI_PROGDIRPATH}/anon_ftp/Downloads
		chmod 777 ${PBI_PROGDIRPATH}/anon_ftp/Uploads
		chmod 755 ${PBI_PROGDIRPATH}/anon_ftp/Downloads
		mv ${PBI_PROGDIRPATH}/anon_ftp /usr/home
		touch Success
	fi
	if [ "$USERS" != "a" ] 
	then 	
		sed -i.bak 's/local_enable=NO/local_enable=YES/g' ${PBI_PROGDIRPATH}/vsftpd.conf
	fi
else 
fi
