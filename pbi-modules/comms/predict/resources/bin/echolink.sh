#!/bin/sh
if [ ! -e ~/.echoLinux ] ; then
	mkdir ~/.echoLinux
	echo "server1.echolink.org" > ~/.echoLinux/servers.txt
fi
if [ ! -e ~/.echoLinux/userdata.txt ] ; then
kdialog --error "
You will need an userdata.txt file placed into
~/.echoLinux/userdata.txt
In this format:

VA3DB
Diane Bruce
Nepean ON
YOURPASS

Please look at http://www.echolink.org
for more information on how to register.
"
else
	/usr/local/bin/echogui
fi
exit 0
