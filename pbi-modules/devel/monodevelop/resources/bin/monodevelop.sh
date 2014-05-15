#!/bin/sh
# Wrapper script to ensure that the proper gconf key is set before running

monodev=`echo $0 | sed 's|.sh$||g'`

# Makesure that monospace_font_name is set
chk=`gconftool-2 --get /desktop/gnome/interface/monospace_font_name 2>/dev/null`
if [ -z "$chk" ] ; then
	gconftool-2 --set /desktop/gnome/interface/monospace_font_name --type string "Monospace 8"
fi

${monodev} "$@"
