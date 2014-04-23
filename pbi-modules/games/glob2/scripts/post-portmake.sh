#!/bin/sh


rm /tmp/glest-new.ini >/dev/null 2>/dev/null
FOUNDLINE="0"

while read line
do

  echo $line | grep Font >/dev/null 2>/dev/null
  if [ "$?" = "0" ]
  then
     if [ "$FOUNDLINE" = "0" ]
     then
     echo "FontConsole=-bitstream-bitstream charter-*-*-normal--*-*-*-*-*-*-*-*
FontDisplay=-bitstream-bitstream charter-*-*-normal--*-*-*-*-*-*-*-*
FontMenu=-bitstream-bitstream charter-*-*-normal--*-*-*-*-*-*-*-*" >> /tmp/glest-new.ini
       FOUNDLINE="1"
     fi
  else
     echo "$line" >> /tmp/glest-new.ini
  fi

done < "${PBI_PROGDIRPATH}/share/glest/glest.ini"

mv /tmp/glest-new.ini ${PBI_PROGDIRPATH}/share/glest/glest.ini

