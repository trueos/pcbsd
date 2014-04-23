#!/bin/sh
# Helper script to make FF the default browser for a user
# Should work for KDE3 and KDE4.
##############################################################################

PROG="!/Programs/bin/firefox3"
FILE="./share/config/kdeglobals"

for D in "${HOME}/.kde" "${HOME}/.kde4"
do
    cd $D 2>/dev/null || break;

    if [ ! -e "$FILE" ]
    then
        echo "ERROR: No kdeglobals file, unable to set $PROG as default"
        exit 1
    fi

    # Check if we already have the BrowserApplication line and modify, or add it
    grep "^BrowserApplication" ${FILE} >/dev/null 2>/dev/null
    if [ "$?" = "0" ]
    then
      ed -s "$FILE" <<EOF
/\[General\]/
/BrowserApplication.*=/
s|=.*|=${PROG}|
wq
EOF
    else
       ULANG="`grep Language= ${FILE} | cut -d '=' -f 2`"
      ed -s "$FILE" <<EOF
/\[General\]/
a
BrowserApplication[${ULANG}]=${PROG}
.
wq
EOF
    fi

done

exit 0
