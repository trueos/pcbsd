#!/bin/sh

# version 1.0.1 (13.Oct.2008)
# * as the 2nd preference (as it doesn't work perfectly yet), try to locate kommander (KDE4) as kmdr-executor

# version 1.0 (02.Apr.2008)
# * don't search bat.kmdr from the directory of kmdr-executor
# * allow passing directory of bat.kmdr as $1

# kmdr-executor -- KDE3 ;; kommander -- KDE4
kmdr_exec="`which kmdr-executor || which kommander 2>/dev/null`"

if [ "$kmdr_exec" ]; then
	script_dir="`dirname $0`"
	for d in $1 ./ $script_dir/../; do
		if [ -f "$d/bat.kmdr" ]; then
			echo "Found bat.kmdr from $d!"
			bat_kmdr_dir="$d"
			break
		fi
	done
	if [ ! "$bat_kmdr_dir" ]; then
		msg="Script bat.kmdr was not found!"
		fallback=1
	else
		$kmdr_exec "$bat_kmdr_dir/bat.kmdr"
	fi
else
	msg="Kommander executor 'kmdr-executor' not found (it's in the kdewebdev package)!"
	fallback=1
fi

if [ "$fallback" = "1" ]; then
	bat_sh="`which bat.sh 2>/dev/null`"
	if [ "$bat_sh" ]; then
		msg="$msg\nFalling back on shell-script!"
		kdialog --sorry "$msg"
		konsole -e bat.sh
	else
		msg="$msg\nScript bat.sh was not found either - cannot continue!"
		kdialog --error "$msg"
	fi
fi
