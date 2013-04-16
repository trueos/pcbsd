#!/bin/sh

MEM=`sysctl hw.realmem | /bin/cut -d " " -f 2`
# Divide a few times to get the size in MB
MEM="`/bin/expr $MEM / 1024`"
MEM="`/bin/expr $MEM / 1024`"

# Lets check and see if we have enough RAM to copy the .uzip file
MDSIZE="`/bin/du -m /dist/PCBSD.ufs.uzip | /bin/cut -f 1`"
MDSIZE="`/bin/expr ${MDSIZE} + 300`"
MDCHECK="`/bin/expr ${MDSIZE} + 512`"
if [ $MEM -lt $MDCHECK ]
then
  # We will run the uzip file from DVD/USB directly, boo!
  echo "Mounting PCBSD.ufs.uzip.."
  # Mount the  "usr" directory
  echo "Mounting /usr"
  MDDEVICE="`/sbin/mdconfig -a -t vnode -o readonly -f /dist/PCBSD.ufs.uzip`.uzip"
  /sbin/mount -r /dev/$MDDEVICE /mntuzip
  # Make the kernel available
  /sbin/mdmfs -S -s 5M -O space md2 /memfs

else
  # We have enough space to copy the uzip file to memory and run from RAM, yay!

  echo "Creating ${MDSIZE}MB ramdisk..."
  /sbin/mdmfs -S -s ${MDSIZE}M -O space md2 /memfs
  echo "Copying LIVE image into memory... This will take a while, please wait (ctrl-t for status)..."
  /bin/cp /dist/PCBSD.ufs.uzip /memfs/

  echo "Mounting /usr"
  mkdir /mntuzip >/dev/null 2>/dev/null
  MDDEVICE="`/sbin/mdconfig -a -t vnode -o readonly -f /memfs/PCBSD.ufs.uzip`.uzip"
  /sbin/mount -r /dev/$MDDEVICE /mntuzip
fi

/sbin/mdmfs -S -s 2M -O space md5 /usr
# Make /usr symlinks
cd /mntuzip/usr
for i in `ls`
do
  ln -s /mntuzip/usr/$i /usr/$i
done

# Load the extra klds we need
/etc/rc.d/kld start

# Check for optimus / video drivers
/etc/pcbsd-video.sh

# See if we have install / meta pkgs for the front-end to use 
if [ -e "/dist/no-meta-pkgs" ] ; then touch /tmp/no-meta-pkgs ; fi
if [ -e "/dist/no-install-pkgs" ] ; then touch /tmp/no-install-pkgs ; fi
if [ -e "/dist/no-fbsd-release" ] ; then touch /tmp/no-fbsd-release ; fi

# Set the marker that this is a LIVE bootup
touch /usr/pcbsd-live
