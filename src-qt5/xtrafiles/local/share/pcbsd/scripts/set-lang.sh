#!/bin/sh
# Set the system language
#

#1 = Language code
#2 = Keyboard layout

echo "$1" >/usr/local/share/pcbsd/conf/system-lang

# Get the full language code
echo "$1" | grep -q "_"
if [ $? -eq 0 ] ; then
  FLANG="${1}.UTF-8"
else
  case ${1} in
     en) up="US" ;;
      *) up=`echo ${1} | tr '[:lower:]' '[:upper:]'` ;;
  esac
  FLANG="${1}_${up}.UTF-8"
fi

# Adjust /etc/login.conf
sed -i '' "s|en_US.UTF-8|${FLANG}|g" /etc/login.conf

exit 0
