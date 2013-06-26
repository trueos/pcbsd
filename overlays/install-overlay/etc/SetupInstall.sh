#!/bin/sh


# Setup the /memfs memory device
echo "Mounting /memfs"

MDSIZE="2"
echo "Creating ${MDSIZE}MB ramdisk..."
/sbin/mdmfs -S -s ${MDSIZE}M -O space md2 /memfs

echo "Mounting /usr"
MDDEVICE="`/sbin/mdconfig -a -t vnode -o readonly -f /uzip/usr.uzip`.uzip"
/sbin/mount -r /dev/$MDDEVICE /usr

PATH="${PATH}:/usr/bin"
export PATH

# Load the extra klds we need
/etc/rc.d/kld start

# Check for optimus / video drivers
/etc/pcbsd-video.sh

# See if we have install / meta pkgs for the front-end to use
if [ -e "/dist/no-meta-pkgs" ] ; then touch /tmp/no-meta-pkgs ; fi
if [ -e "/dist/no-install-pkgs" ] ; then touch /tmp/no-install-pkgs ; fi
if [ -e "/dist/no-fbsd-release" ] ; then touch /tmp/no-fbsd-release ; fi

# Check if we have a pc-autoinstall.conf file
if [ -e "/boot/pc-autoinstall.conf" ]
then
  # We have one, copy it over so that pc-sysinstall can parse it later during the bootup
  cp /boot/pc-autoinstall.conf /tmp/pc-autoinstall.conf
  
  # Check if the config file is on disk as well
  PCCFG=`grep "pc_config:" /tmp/pc-autoinstall.conf | grep -v "^#" | /bin/cut -d ':' -f 2 | tr -d ' '`

  # Check that this isn't a http / ftp file we need to fetch later
  echo "${PCCFG}" | grep -e "^http" -e "^ftp" > /dev/null 2>/dev/null
  if [ "$?" != "0" ]
  then
    if [ -e "/${PCCFG}" ]
    then
      cp /${PCCFG} /tmp/pc-sysinstall.cfg
    else
      echo "ERROR: pc_config: ${PCCFG} isn't on the boot media! Automated install aborted!"
      rm /tmp/pc-autoinstall.conf
    fi 
  fi
  
fi
