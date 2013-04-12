#!/bin/sh

# Enable loading the driver
grep '^vmware_guest_vmblock_enable="YES"' /etc/rc.conf >/dev/null 2>/dev/null
if [ $? -ne 0 ] ; then
	echo 'vmware_guest_vmblock_enable="YES"' >>/etc/rc.conf
fi
grep '^vmware_guest_vmhgfs_enable="YES"' /etc/rc.conf >/dev/null 2>/dev/null
if [ $? -ne 0 ] ; then
	echo 'vmware_guest_vmhgfs_enable="YES"' >>/etc/rc.conf
fi
grep '^vmware_guest_vmmemctl_enable="YES"' /etc/rc.conf >/dev/null 2>/dev/null
if [ $? -ne 0 ] ; then
	echo 'vmware_guest_vmmemctl_enable="YES"' >>/etc/rc.conf
fi
grep '^vmware_guest_vmxnet_enable="YES"' /etc/rc.conf >/dev/null 2>/dev/null
if [ $? -ne 0 ] ; then
	echo 'vmware_guest_vmxnet_enable="YES"' >>/etc/rc.conf
fi
grep '^vmware_guestd_enable="YES"' /etc/rc.conf >/dev/null 2>/dev/null
if [ $? -ne 0 ] ; then
	echo 'vmware_guestd_enable="YES"' >>/etc/rc.conf
fi
