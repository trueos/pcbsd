#!/bin/sh
#
# Name    : Xorg autoconfiguration script
# Version : 0.5
# Author  : Sergei Mozhaisky (techniX)
# Adapted for PC-BSD by Kris Moore

XFREE_CONF="/etc/X11/xorg.conf"
XFREE_NEWCONF="/root/xorg.conf.new"
XFREE_TMPCONF="/root/cardDetect/XF86Config.default"
XFREE_VMWARE="/root/cardDetect/XF86Config.vmware"
XFREE_VBOX="/root/cardDetect/XF86Config.virtualbox"
XFREE_QEMU="/root/cardDetect/XF86Config.qemu"


if [ -f ${XFREE_CONF} ]; then
	echo "Found config file \"${XFREE_CONF}\" - skiping autoconfiguration."
	exit
fi

echo -n "Autoconfiguring X Server..."

VMWARE=`pciconf -lv | grep -i vmware`
QEMU=`pciconf -lv | grep CL-GD5446`
VIRTUALBOX=`pciconf -lv | grep VirtualBox`
if [ "${VMWARE}" != "" ]; then
	cp ${XFREE_VMWARE} ${XFREE_NEWCONF}
elif [ "${VIRTUALBOX}" != "" ]; then
	cp ${XFREE_VBOX} ${XFREE_NEWCONF}
elif [ "$QEMU" != "" ]; then
        cp ${XFREE_QEMU} ${XFREE_NEWCONF}
        # Must enable 16 bit color
        cat ${XFREE_TMPCONF} | sed "s/#%DEFAULTDEPTH%/DefaultDepth 16/" > /tmp/xorg.qemu
        XFREE_TMPCONF="/tmp/xorg.qemu"
else
        # configuring X automatically
	X -configure >/dev/null 2>&1
fi

X_MON=`/usr/bin/sed -n '/^Section "Monitor"$/,/^EndSection$/ p' $XFREE_NEWCONF`
echo "$X_MON" > /tmp/mon
MON_DET=`cat /tmp/mon | grep VendorName | awk  '{print (substr($2,2,7))}'`
rm -f /tmp/mon
if [ "$MON_DET" = "Monitor" ] ; then 
X_MON='
Section "Monitor"
	Identifier   "Monitor0"
	ModelName    "Unprobed Monitor (no DDC)"

        #Info: All lines begining with # are comments
	#Info: Uncomment the the desired lines by removing
        # the  # sign  in front of them.
	#Warning: Dont forget to comment the unused lines
	# by adding a # sign in front of them
        
	HorizSync    28.0 - 60.0 # Warning: This is for very old Monitors
#	HorizSync    28.0 - 78.0 # Warning: This may fry olders Monitors
#	HorizSync    28.0 - 96.0 # Warning: This may fry old Monitors
         
                    #Info: TFT default or very old CRT Monitors
        VertRefresh  50.0 - 60.0 # Extreme conservative. Will flicker.
                    
	            #Info: TFT Monitors or olders CRT Monitors
#	VertRefresh  50.0 - 76.0 # Very conservative. May flicker.

		    #Info: Only for CRT monitors
#	VertRefresh  50.0 - 100.0 # Not conservative. It will not flicker.

	#  Default modes distilled from
	#      "VESA and Industry Standards and Guide for Computer Display Monitor
	#       Timing", version 1.0, revision 0.8, adopted September 17, 1998.
	#  $XFree86: xc/programs/Xserver/hw/xfree86/etc/vesamodes,v 1.4 1999/11/18 16:52:17 tsi Exp $
	# 640x350 @ 85Hz (VESA) hsync: 37.9kHz
	ModeLine "640x350"    31.5  640  672  736  832    350  382  385  445 +hsync -vsync
	# 640x400 @ 85Hz (VESA) hsync: 37.9kHz
	ModeLine "640x400"    31.5  640  672  736  832    400  401  404  445 -hsync +vsync
	# 720x400 @ 85Hz (VESA) hsync: 37.9kHz
	ModeLine "720x400"    35.5  720  756  828  936    400  401  404  446 -hsync +vsync
	# 640x480 @ 60Hz (Industry standard) hsync: 31.5kHz
	ModeLine "640x480"    25.2  640  656  752  800    480  490  492  525 -hsync -vsync
	# 640x480 @ 72Hz (VESA) hsync: 37.9kHz
	ModeLine "640x480"    31.5  640  664  704  832    480  489  491  520 -hsync -vsync
	# 640x480 @ 75Hz (VESA) hsync: 37.5kHz
	ModeLine "640x480"    31.5  640  656  720  840    480  481  484  500 -hsync -vsync
	# 640x480 @ 85Hz (VESA) hsync: 43.3kHz
	ModeLine "640x480"    36.0  640  696  752  832    480  481  484  509 -hsync -vsync
	# 800x600 @ 56Hz (VESA) hsync: 35.2kHz
	ModeLine "800x600"    36.0  800  824  896 1024    600  601  603  625 +hsync +vsync
	# 800x600 @ 60Hz (VESA) hsync: 37.9kHz
	ModeLine "800x600"    40.0  800  840  968 1056    600  601  605  628 +hsync +vsync
	# 800x600 @ 72Hz (VESA) hsync: 48.1kHz
	ModeLine "800x600"    50.0  800  856  976 1040    600  637  643  666 +hsync +vsync
	# 800x600 @ 75Hz (VESA) hsync: 46.9kHz
	ModeLine "800x600"    49.5  800  816  896 1056    600  601  604  625 +hsync +vsync
	# 800x600 @ 85Hz (VESA) hsync: 53.7kHz
	ModeLine "800x600"    56.3  800  832  896 1048    600  601  604  631 +hsync +vsync
	# 1024x768i @ 43Hz (industry standard) hsync: 35.5kHz
	ModeLine "1024x768"   44.9 1024 1032 1208 1264    768  768  776  817 +hsync +vsync Interlace
	# 1024x768 @ 60Hz (VESA) hsync: 48.4kHz
	ModeLine "1024x768"   65.0 1024 1048 1184 1344    768  771  777  806 -hsync -vsync
	# 1024x768 @ 70Hz (VESA) hsync: 56.5kHz
	ModeLine "1024x768"   75.0 1024 1048 1184 1328    768  771  777  806 -hsync -vsync
	# 1024x768 @ 75Hz (VESA) hsync: 60.0kHz
	ModeLine "1024x768"   78.8 1024 1040 1136 1312    768  769  772  800 +hsync +vsync
	# 1024x768 @ 85Hz (VESA) hsync: 68.7kHz
	ModeLine "1024x768"   94.5 1024 1072 1168 1376    768  769  772  808 +hsync +vsync
	# 1024x768 @ 100.00 Hz (GTF) hsync: 81.40 kHz; pclk: 113.31 MHz
	ModeLine "1024x768"  113.31  1024 1096 1208 1392  768 769 772 814  -HSync +Vsync
EndSection
'
fi

X_DEV=`/usr/bin/sed -n '/^Section "Device"$/,/^EndSection$/ p' $XFREE_NEWCONF`

# Use ati driver instead of radeon for installation
X_DEV=`echo "$X_DEV" | sed 's|radeon|ati|g'`

# Use the "nvidia" driver instead of "nv"
#X_DEV=`echo "$X_DEV" | sed 's|nv|nvidia|g'`

case "${X_DEV:-NULL}" in
    NULL)
X_DEV='
Section "Device"
        Identifier      "Card0"
	Driver          "vesa"
EndSection
'
;;
esac

cp $XFREE_TMPCONF $XFREE_CONF
printf "$X_MON\n\n" >> $XFREE_CONF
printf "$X_DEV\n\n" >> $XFREE_CONF
/usr/bin/touch /root/.disable
rm $XFREE_NEWCONF
echo "done."

