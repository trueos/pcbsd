#!/bin/sh

# Copy over the necessary bits
mkdir tmpmnt
cp -r /lib tmpmnt/
cp -r /libexec tmpmnt/
cp -r /bin tmpmnt/
cp -r /sbin tmpmnt/
cp /usr/lib/libgnuregex* tmpmnt/lib/
cp /usr/lib/libbz2* tmpmnt/lib/
cp /usr/bin/grep tmpmnt/bin/
cp /usr/bin/awk tmpmnt/bin/
cp /usr/bin/sed tmpmnt/bin/
cp /usr/bin/rev tmpmnt/bin/
cp /usr/bin/cut tmpmnt/bin/
cp /usr/bin/tput tmpmnt/bin/
cp /usr/bin/clear tmpmnt/bin/
mkdir tmpmnt/dev
mkdir tmpmnt/tmp
mkdir tmpmnt/var
mkdir tmpmnt/boot
mkdir tmpmnt/etc
mkdir tmpmnt/mnt
mkdir -p tmpmnt/usr/share/misc
touch tmpmnt/etc/fstab
cp /usr/share/misc/termcap tmpmnt/usr/share/misc
ln -s /usr/share/misc/termcap tmpmnt/etc/termcap
cp /etc/master.passwd tmpmnt/etc/
cp /etc/passwd tmpmnt/etc/
cp /etc/group tmpmnt/etc/
cp /etc/spwd.db tmpmnt/etc/
cp /etc/pwd.db tmpmnt/etc/
cp /etc/login.conf tmpmnt/etc/
cp rc tmpmnt/etc/rc

# Create the FS now
makefs -b 6000 -o label=USBRESIZE mfsroot tmpmnt
gzip -9 mfsroot
mv mfsroot.gz /boot/

# Enable the new mfsroot loading
echo "rootfs_load=\"YES\"" >> /boot/loader.conf
echo "rootfs_type=\"mfs_root\"" >> /boot/loader.conf
echo "rootfs_name=\"/boot/mfsroot\"" >> /boot/loader.conf
echo "vfs.root.mountfrom=\"ufs:md0\"" >> /boot/loader.conf

# Remove tmp
chflags -R noschg tmpmnt
rm -rf tmpmnt
