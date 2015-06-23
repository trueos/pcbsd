#!/bin/sh

# Set shell to ignore SIGINT (2), but not children;
trap : 2

# Do a check of available memory  
sysMem=`/sbin/sysctl hw.realmem | /bin/cut -d ' ' -f 2`
minSize=`/bin/expr 250 \* 1024 \* 1024`
if [ $sysMem -lt $minSize ] ; then
        echo "Warning: PC-BSD requires a minimum of 256MB of RAM for optimal performance."
        echo "You may experience problems with less than this ammount."
        echo ""
        echo "Press ENTER to continue"
        read tmp
fi

# Load geom_uzip
kldload geom_uzip

# Create a /tmp file system on tmpfs
kldload tmpfs
mount -t tmpfs tmpfs /tmp

# If we have a LIVE DVD image, boot with it
if [ -e "/dist/PCBSD.ufs.uzip" ]
then
  MEM=`sysctl hw.realmem | /bin/cut -d " " -f 2`

  # We are running in LIVE mode
  sh /etc/SetupLive.sh
else
  # Installation medium ONLY

  # We are running a regular Install
  sh /etc/SetupInstall.sh
fi

# Extract the etc-dist.txz
/sbin/mdmfs -S -s 6M -O space md10 /etc
/usr/bin/tar xvf /uzip/etc-dist.txz -C /etc >/dev/null 2>/dev/null

# Extract the var-dist.txz
/sbin/mdmfs -S -s 10M -O space md9 /var
/usr/bin/tar xvf /uzip/var-dist.txz -C /var >/dev/null 2>/dev/null

# Extract the root-dist.txz
/sbin/mdmfs -S -s 15M -O space md8 /root
/usr/bin/tar xvf /uzip/root-dist.txz -C /root >/dev/null 2>/dev/null
