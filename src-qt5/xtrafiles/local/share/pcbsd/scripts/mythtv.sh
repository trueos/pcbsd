#!/bin/sh
# Script to start mythtv and init

SCRIPT="/usr/local/share/pcbsd/scripts/mythtv.sh"

# Make sure we are running as root
as_root() {
	if [ "`id -u`" != "0" ] ; then
		echo "ERROR: Must be run as root!"
		exit
	fi
}

check_init()
{
	if [ ! -e "/var/log/mythinit.log" -o "$1" = "init" ] ; then
		pc-su xterm -e ${SCRIPT} doinit
	fi
}

do_init() 
{
	as_root
	echo "Starting mysql-server..."
	/usr/local/etc/rc.d/mysql-server stop >/dev/null 2>/dev/null
	killall -9 mysqld >/dev/null 2>/dev/null
	killall -9 mysqld_safe >/dev/null 2>/dev/null
	/usr/local/etc/rc.d/mysql-server start >/dev/null 2>/dev/null

	# Do we need a password?
	echo "?;" | mysql --user=root >/dev/null 2>/dev/null
	if [ $? -eq 0 ] ; then PASS="" ; else PASS="-p" ; fi
	
	echo "Creating mysql database / user..."
	mysql --user=root $PASS < /usr/local/share/mythtv/database/mc.sql

	# Enable the tzinfo
	mysql_tzinfo_to_sql /usr/share/zoneinfo | mysql -uroot ${PASS} mysql

	echo "Preparing to start mythtv setup..."
	do_setup

	echo "Starting MythTV backend..."
	/usr/local/etc/rc.d/mythbackend stop >/dev/null 2>/dev/null
	/usr/local/etc/rc.d/mythbackend start >/dev/null 2>/dev/null
	echo "MythTV configured `date`" > /var/log/mythinit.log

	echo "Finished! Press ENTER to continue."
	read tmp
	exit 0
}

# Run the setup program
do_setup() 
{
	as_root
	mythtv-setup
	/usr/local/etc/rc.d/mythbackend stop >/dev/null 2>/dev/null
	/usr/local/etc/rc.d/mythbackend start >/dev/null 2>/dev/null
}

check_setup()
{
	if [ "$1" = "setup" ] ; then
		pc-su xterm -e ${SCRIPT} dosetup
	fi
}

check_installed()
{
	which mythfrontend >/dev/null 2>/dev/null
	if [ $? -ne 0 ] ; then
		echo "ERROR: MythTV is not installed!" 
		exit 1
	fi
};

check_backend()
{
	# Lets check if the backend is running
	ps -awwxc | grep mythbackend >/dev/null 2>/dev/null
	if [ $? -ne 0 ] ; then
		zenity --question --text "The mythbackend daemon is not running. Do you wish to re-run the configuration? "
		if [ $? -eq 1 ] ; then
			exit 1
		fi
		check_setup setup
		exit 0
	fi
};

check_installed

case $1 in
	doinit) do_init ; exit 0 ;;
	dosetup) do_setup ; exit 0 ;;
	*) ;;
esac

check_init $1
check_setup $1
check_backend 

mythfrontend 
