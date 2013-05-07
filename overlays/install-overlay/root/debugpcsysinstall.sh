#!/bin/sh

cp -r /usr/local/share/pc-sysinstall /root/
cp /usr/local/sbin/pc-sysinstall /root/pc-sysinstall/
sed -i '' 's|/usr/local/share/pc-sysinstall|/root/pc-sysinstall|g' /root/pc-sysinstall/pc-sysinstall

echo "pc-sysinstall cloned and ready in /root/pc-sysinstall/"
