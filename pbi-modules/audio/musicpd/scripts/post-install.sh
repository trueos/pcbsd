#!/bin/sh

mkdir -p /var/mpd/music >/dev/null 2>/dev/null
mkdir -p /var/mpd/.mpd/playlists >/dev/null 2>/dev/null
#chmod 777 /var/mpd/music
chown -R mpd:mpd /var/mpd

#ln -s /var/mpd/Music ${PBI_PROGDIRPATH}/Music


echo "musicpd_enable=\"YES\"" >>/etc/rc.conf
ln -s ${PBI_PROGDIRPATH}/etc/rc.d/musicpd ${PBI_RCDIR}/musicpd
${PBI_RCDIR}/musicpd start

if [ ! -z "${DISPLAY}" ]
then
else
  echo "MPD is a daemon and has no interface, to use install a front-end such as GMPC or other."
fi


