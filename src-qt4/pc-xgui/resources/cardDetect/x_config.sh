#!/bin/sh
#
# Name    : XFree86 autoconfiguration script
# Version : 0.1
# Author  : Sergei Mozhaisky (techniX)
# Adapted for RoFreeSBIE CD by Angelescu Ovidiu

XFREE_CONF="/etc/X11/xorg.conf"
XFREE_NEWCONF="/root/xorg.conf.new"
XFREE_TMPCONF="/usr/local/share/pcbsd/xorg-gui/cardDetect/XF86Config.default"
XFREE_VMWARE="/usr/local/share/pcbsd/xorg-gui/cardDetect/XF86Config.vmware"

if [ -f ${XFREE_CONF} ]; then
	echo "Found config file \"${XFREE_CONF}\" - skiping autoconfiguration."
	exit
fi

echo -n "Autoconfiguring X Server..."

VMWARE=`pciconf -lv | grep -i vmware`
if [ "${VMWARE}" != "" ]; then
	cp ${XFREE_VMWARE} ${XFREE_NEWCONF}
else
        # configuring X automatically
	/usr/local/bin/X -configure >/dev/null 2>&1
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
        
#	HorizSync    28.0 - 60.0 # Warning: This is for very old Monitors
	HorizSync    28.0 - 78.0 # Warning: This may fry olders Monitors
#	HorizSync    28.0 - 96.0 # Warning: This may fry old Monitors
         
                    #Info: TFT default or very old CRT Monitors
#        VertRefresh  50.0 - 60.0 # Extreme conservative. Will flicker.
                    
	            #Info: TFT Monitors or olders CRT Monitors
	VertRefresh  50.0 - 76.0 # Very conservative. May flicker.

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
	# 1152x864 @ 75Hz (VESA) hsync: 67.5kHz
	ModeLine "1152x864"  108.0 1152 1216 1344 1600    864  865  868  900 +hsync +vsync
	# 1280x960 @ 60Hz (VESA) hsync: 60.0kHz
	ModeLine "1280x960"  108.0 1280 1376 1488 1800    960  961  964 1000 +hsync +vsync
	# 1280x960 @ 85Hz (VESA) hsync: 85.9kHz
	ModeLine "1280x960"  148.5 1280 1344 1504 1728    960  961  964 1011 +hsync +vsync
	# 1280x1024 @ 60Hz (VESA) hsync: 64.0kHz
	ModeLine "1280x1024" 108.0 1280 1328 1440 1688   1024 1025 1028 1066 +hsync +vsync
	# 1280x1024 @ 75Hz (VESA) hsync: 80.0kHz
	ModeLine "1280x1024" 135.0 1280 1296 1440 1688   1024 1025 1028 1066 +hsync +vsync
	# 1280x1024 @ 85Hz (VESA) hsync: 91.1kHz
	ModeLine "1280x1024" 157.5 1280 1344 1504 1728   1024 1025 1028 1072 +hsync +vsync
	# 1600x1200 @ 60Hz (VESA) hsync: 75.0kHz
	ModeLine "1600x1200" 162.0 1600 1664 1856 2160   1200 1201 1204 1250 +hsync +vsync
	# 1600x1200 @ 65Hz (VESA) hsync: 81.3kHz
	ModeLine "1600x1200" 175.5 1600 1664 1856 2160   1200 1201 1204 1250 +hsync +vsync
	# 1600x1200 @ 70Hz (VESA) hsync: 87.5kHz
	ModeLine "1600x1200" 189.0 1600 1664 1856 2160   1200 1201 1204 1250 +hsync +vsync
	# 1600x1200 @ 75Hz (VESA) hsync: 93.8kHz
	ModeLine "1600x1200" 202.5 1600 1664 1856 2160   1200 1201 1204 1250 +hsync +vsync
	# 1600x1200 @ 85Hz (VESA) hsync: 106.3kHz
	ModeLine "1600x1200" 229.5 1600 1664 1856 2160   1200 1201 1204 1250 +hsync +vsync
	# 1792x1344 @ 60Hz (VESA) hsync: 83.6kHz
	ModeLine "1792x1344" 204.8 1792 1920 2120 2448   1344 1345 1348 1394 -hsync +vsync
	# 1792x1344 @ 75Hz (VESA) hsync: 106.3kHz
	ModeLine "1792x1344" 261.0 1792 1888 2104 2456   1344 1345 1348 1417 -hsync +vsync
	# 1856x1392 @ 60Hz (VESA) hsync: 86.3kHz
	ModeLine "1856x1392" 218.3 1856 1952 2176 2528   1392 1393 1396 1439 -hsync +vsync
	# 1856x1392 @ 75Hz (VESA) hsync: 112.5kHz
	ModeLine "1856x1392" 288.0 1856 1984 2208 2560   1392 1393 1396 1500 -hsync +vsync
	# 1920x1440 @ 60Hz (VESA) hsync: 90.0kHz
	ModeLine "1920x1440" 234.0 1920 2048 2256 2600   1440 1441 1444 1500 -hsync +vsync
	# 1920x1440 @ 75Hz (VESA) hsync: 112.5kHz
	ModeLine "1920x1440" 297.0 1920 2064 2288 2640   1440 1441 1444 1500 -hsync +vsync
	# Additional modelines
	ModeLine "1800x1440"  230    1800 1896 2088 2392  1440 1441 1444 1490 +HSync +VSync
	ModeLine "1800x1440"  250    1800 1896 2088 2392  1440 1441 1444 1490 +HSync +VSync
	# Extended modelines with GTF timings
	# 640x480 @ 100.00 Hz (GTF) hsync: 50.90 kHz; pclk: 43.16 MHz
	ModeLine "640x480"  43.16  640 680 744 848  480 481 484 509  -HSync +Vsync
	# 768x576 @ 60.00 Hz (GTF) hsync: 35.82 kHz; pclk: 34.96 MHz
	ModeLine "768x576"  34.96  768 792 872 976  576 577 580 597  -HSync +Vsync
	# 768x576 @ 72.00 Hz (GTF) hsync: 43.27 kHz; pclk: 42.93 MHz
	ModeLine "768x576"  42.93  768 800 880 992  576 577 580 601  -HSync +Vsync
	# 768x576 @ 75.00 Hz (GTF) hsync: 45.15 kHz; pclk: 45.51 MHz
	ModeLine "768x576"  45.51  768 808 888 1008  576 577 580 602  -HSync +Vsync
	# 768x576 @ 85.00 Hz (GTF) hsync: 51.42 kHz; pclk: 51.84 MHz
	ModeLine "768x576"  51.84  768 808 888 1008  576 577 580 605  -HSync +Vsync
	# 768x576 @ 100.00 Hz (GTF) hsync: 61.10 kHz; pclk: 62.57 MHz
	ModeLine "768x576"  62.57  768 816 896 1024  576 577 580 611  -HSync +Vsync
	# 800x600 @ 100.00 Hz (GTF) hsync: 63.60 kHz; pclk: 68.18 MHz
	ModeLine "800x600"  68.18  800 848 936 1072  600 601 604 636  -HSync +Vsync
	# 1024x768 @ 100.00 Hz (GTF) hsync: 81.40 kHz; pclk: 113.31 MHz
	ModeLine "1024x768"  113.31  1024 1096 1208 1392  768 769 772 814  -HSync +Vsync
	# 1152x864 @ 60.00 Hz (GTF) hsync: 53.70 kHz; pclk: 81.62 MHz
	ModeLine "1152x864"  81.62  1152 1216 1336 1520  864 865 868 895  -HSync +Vsync
	# 1152x864 @ 85.00 Hz (GTF) hsync: 77.10 kHz; pclk: 119.65 MHz
	ModeLine "1152x864"  119.65  1152 1224 1352 1552  864 865 868 907  -HSync +Vsync
	# 1152x864 @ 100.00 Hz (GTF) hsync: 91.50 kHz; pclk: 143.47 MHz
	ModeLine "1152x864"  143.47  1152 1232 1360 1568  864 865 868 915  -HSync +Vsync
	# 1280x960 @ 72.00 Hz (GTF) hsync: 72.07 kHz; pclk: 124.54 MHz
	ModeLine "1280x960"  124.54  1280 1368 1504 1728  960 961 964 1001  -HSync +Vsync
	# 1280x960 @ 75.00 Hz (GTF) hsync: 75.15 kHz; pclk: 129.86 MHz
	ModeLine "1280x960"  129.86  1280 1368 1504 1728  960 961 964 1002  -HSync +Vsync
	# 1280x960 @ 100.00 Hz (GTF) hsync: 101.70 kHz; pclk: 178.99 MHz
	ModeLine "1280x960"  178.99  1280 1376 1520 1760  960 961 964 1017  -HSync +Vsync
	# 1280x1024 @ 100.00 Hz (GTF) hsync: 108.50 kHz; pclk: 190.96 MHz
	ModeLine "1280x1024"  190.96  1280 1376 1520 1760  1024 1025 1028 1085  -HSync +Vsync
	# 1400x1050 @ 60.00 Hz (GTF) hsync: 65.22 kHz; pclk: 122.61 MHz
	ModeLine "1400x1050"  122.61  1400 1488 1640 1880  1050 1051 1054 1087  -HSync +Vsync
	# 1400x1050 @ 72.00 Hz (GTF) hsync: 78.77 kHz; pclk: 149.34 MHz
	ModeLine "1400x1050"  149.34  1400 1496 1648 1896  1050 1051 1054 1094  -HSync +Vsync
	# 1400x1050 @ 75.00 Hz (GTF) hsync: 82.20 kHz; pclk: 155.85 MHz
	ModeLine "1400x1050"  155.85  1400 1496 1648 1896  1050 1051 1054 1096  -HSync +Vsync
	# 1400x1050 @ 85.00 Hz (GTF) hsync: 93.76 kHz; pclk: 179.26 MHz
	ModeLine "1400x1050"  179.26  1400 1504 1656 1912  1050 1051 1054 1103  -HSync +Vsync
	# 1400x1050 @ 100.00 Hz (GTF) hsync: 111.20 kHz; pclk: 214.39 MHz
	ModeLine "1400x1050"  214.39  1400 1512 1664 1928  1050 1051 1054 1112  -HSync +Vsync
	# 1600x1200 @ 100.00 Hz (GTF) hsync: 127.10 kHz; pclk: 280.64 MHz
	ModeLine "1600x1200"  280.64  1600 1728 1904 2208  1200 1201 1204 1271  -HSync +Vsync
EndSection
'
fi

X_DEV=`/usr/bin/sed -n '/^Section "Device"$/,/^EndSection$/ p' $XFREE_NEWCONF`

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

