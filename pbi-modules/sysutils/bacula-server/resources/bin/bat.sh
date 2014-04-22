#!/bin/sh

# script by Silver Salonen

# version 2.0 (14.Mar.2008)
# * use functions for clearing up the picture
# * make the script usable for Kommander - see new options in usage()
# * fix bat's full path in PBI-directory

# version 1.1 (28.Feb.2008)
# * adjust the script for using it in PBI :)
# * allow bat's binary to be specified manually in script and add some alternatives
# * in order to achieve this, some variables had to be renamed
# * search configurations from ~/.bacula-bat by default
# * don't assume the "bat-" prefix for configurations files

# version 1.0 (25.Sep.2007)


while getopts "c:f:lLd:hs:" opt; do
	case $opt in
		c)
			bat_config="$OPTARG"
		;;
		f)
			bat_config_file="$OPTARG"
		;;
		d)
			configs_path="$OPTARG"
		;;
		l)
			listOnly=1
		;;
		L)
			listNumberedOnly=1
		;;
		s)
			sleep="$OPTARG"
		;;
		h)
			helpOnly=1
		;;
	esac
done

if [ ! "$sleep" ]; then
	sleep=2
fi

# expects arguments: <message>
error() {
	local message="$1"
	echo "$message"
	sleep $sleep
	exit 1
}

if [ ! "$configs_path" ]; then
	configs_path=~/.bacula-bat/
else
	configs_path="${configs_path}/"
fi
if [ ! -d "$configs_path" ]; then
	error "Error! Directory '$configs_path' for Bat configurations not found!"
fi

usage() {
	echo "Usage: $0 ( -c <config> | -f <config-file> | -l | -L ) [ -d <config-path> ] [ -s <seconds> ] [ -h ]"
	echo ""
	echo "Options:"
	echo "	-d <directory>		specify directory containing Bat configurations. Default: ~/.bacula-bat/"
	echo "	-c <config>		execute Bat with specified configuration (\$configs_path/<configuration>.conf)"
	echo "	-f <config-file>	execute Bat with specified configuration-file"
	echo "	-l			only display a list of configurations"
	echo "	-L			only display a numbered list of configurations"
	echo "	-h			only show help"
	echo "	-s <seconds>		sleep for <seconds> in case of any error. Default: 2"
	echo ""
	echo "If neither -c, -f, -l nor -L is used, list configurations and let choose amongst them."
}

whichBat() {
	local bat_pbi=/Programs/BaculaBat*/bin/bat
	local bin=
	for bin in ./bat bin/bat bat $bat_pbi; do
		bin=`which "$bin"`
		if [ "$bin" ] && [ -x "$bin" ]; then
			echo $bin
			break
		fi
	done

	if [ ! "$bin" ]; then
		exit 1
	fi
}

# expects arguments: <path>
getConfigs() {
	local configs_path="$1"
	local bats_confs="`ls -1 "${configs_path}"*.conf 2>/dev/null | sed "s|.*/||" | sed "s|.conf$||"`"
	if [ ! "$bats_confs" ]; then
		exit 1
	fi
	echo "$bats_confs"
}

# expects arguments: <configurations>
listConfigs()  {
	local bats_confs="$1"
	local n=0
	local list_confs=
	for bat_conf in $bats_confs; do
		n=$(($n + 1))
		list_confs="$list_confs
$n: $bat_conf"
	done
	echo "$list_confs" | sed "1d"
}

# ===========================
# OK, let the script begin :)
# ===========================

bat=`whichBat` || error "Where's bat? :("
bats_confs="`getConfigs "$configs_path"`" || error "Error! Could not find any *.conf files!"
# populate array of configurations
n=0
for bat_conf in $bats_confs; do
	n=$(($n + 1))
	eval bat_conf$n=$bat_conf
done

# print help
if [ "$helpOnly" ]; then
	usage
	exit
fi

# only list the configurations
if [ "$listOnly" ]; then
	echo "$bats_confs"
	exit
fi

list_confs="`listConfigs "$bats_confs"`"

# only list the configurations
if [ "$listNumberedOnly" ]; then
	echo "$list_confs"
	exit
fi

# ask which configuration to use
if [ ! "$bat_config_file" ] && [ ! "$bat_config" ]; then
	answer="invalid ;)"
	while [ "$answer" != "" ] && [ ! "$bat_config" ]; do
		echo -n "$list_confs

Enter the number of Bacula server (empty to cancel): "
		read answer
		eval bat_config=\$bat_conf$answer
	done

	if [ "$answer" = "" ]; then
		error "OK, bye :)"
	fi
fi

if [ ! "$bat_config_file" ]; then
	eval bat_config_file="${configs_path}${bat_config}.conf"
fi

echo "Found Bat's binary: $bat"
echo "Using configuration-file: $bat_config_file"
$bat -c "$bat_config_file"
if [ ! $? -eq 0 ]; then
	sleep $sleep
fi
