#!/bin/sh

cd /home
for i in `ls`
do
 if [ ! -e "/home/${i}/.audacity" ]
 then
   echo "Version=1.2.4
PrefsVersion=1.1.1r1
[Directories]
TempDir=/home/${i}/.audacity_temp/
[MP3]
MP3LibPath=${PBI_PROGDIRPATH}/autolibs/libmp3lame.so
[Prefs]
PrefsCategory=4" > /home/${i}/.audacity
  chmod 644 /home/$i/.audacity 
  chown ${i}:${i} /home/${i}/.audacity

  mkdir /home/${i}/.audacity_temp
  chown ${i}:${i} /home/${i}/.audacity_temp
 fi
done

