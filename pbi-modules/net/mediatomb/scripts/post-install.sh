#!/bin/sh

echo "TOTALSTEPS: 4"
echo "SETSTEPS: 1"

# Something links LOCALBASE/rc.d/mediatomb to
# PBI_PROGDIRPATH/etc/rc.d/mediatomb, which causes a nasty execution
# recursion with the wrapper script. We blow it out.
rm -f ${PBI_RCDIR}/mediatomb

cat <<EOF > ${PBI_RCDIR}/mediatomb
#!/bin/sh
export LD_LIBRARY_PATH=${PBI_PROGDIRPATH}/lib:\$LD_LIBRARY_PATH
${PBI_PROGDIRPATH}/etc/rc.d/mediatomb \$@
EOF

chmod +x ${PBI_RCDIR}/mediatomb

echo "SETSTEPS: 2"
echo "MSG: Configuring"
echo "mediatomb_enable=\"YES\"" >> /etc/rc.conf

/usr/sbin/pw groupadd mediatomb
/usr/sbin/pw useradd mediatomb -g mediatomb -h - \
                     -s "/sbin/nologin" -d "/nonexistent"  \
                     -c "MediaTomb"

chown mediatomb:mediatomb ${PBI_PROGDIRPATH}/etc/mediatomb/config*
chgrp mediatomb ${PBI_PROGDIRPATH}/bin/mediatomb
chgrp mediatomb ${PBI_PROGDIRPATH}/etc/mediatomb
mkdir /var/mediatomb
chown -R mediatomb:mediatomb /var/mediatomb

sed -i.bak s,/usr/local/bin,${PBI_PROGDIRPATH}/bin,g ${PBI_PROGDIRPATH}/etc/mediatomb/config.xml

echo "SETSTEPS: 3"
echo "MSG: Starting mediatomb service"
${PBI_RCDIR}/mediatomb start

echo "MSG: Finished!"
echo "SETSTEPS: 4"

