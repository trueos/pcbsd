#!/bin/sh

PATH="/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin" ; export PATH
HOME="/root" ; export HOME
TERM="cons25" ; export TERM

# Clear the screen first
clear

# Check if we are running as an install server
if [ -e "/etc/installserver" ] ; then
  /etc/scripts/tcsinstall.sh
  halt -p
fi

# Restart moused
/etc/rc.d/moused restart

# Check if given option to use vesa mode at boot splash
xvesa="NO"
v=`/bin/kenv xvesa`
if [ $? -eq 0 ]
then
   xvesa=$v
fi


# Figure out which NIC we are using
NIC="`ifconfig | grep '<UP' | grep -v '^lo0' | cut -d ':' -f 1`"
echo "Running on NIC: $NIC"

# Now get the MAC address of this NIC
MAC="`ifconfig ${NIC} | grep ether | cut -d ' ' -f 2`"

# Now check if we have a saved xorg.conf file for this MAC
if [ -e "/mnt/xorg-config/${MAC}.conf" ]
then
  if [ "$xvesa" = "YES" ]
  then
    echo "Running in Failsafe VESA mode."
    cp /etc/scripts/cardDetect/XF86Config.compat /etc/X11/xorg.conf
  else 
    echo "Using saved xorg config for $MAC"
    cp /mnt/xorg-config/${MAC}.conf /etc/X11/xorg.conf 
  fi

  # Now bring up the X session to query for XDM servers
  /usr/local/bin/X -broadcast
  reboot
else
  echo "No saved xorg-config for this MAC: $MAC"
  echo "Do you wish to setup a custom xorg.conf for this system? (Y/N)"
  read -t10s response 
  if [ "${response}" = "Y" -o "${response}" = "y" ]
  then
     while
     z=1
     do
     dialog --menu 'Select your setup option' 15 55 10 'vesa' 'Use failsafe VESA mode' 'auto' 'Run X auto-detection' 'viconfig' 'Edit xorg.conf with vi' 'test' 'Test Configuration' 'shell' 'Run Shell' 'save' 'Save working config' 'close' 'Close and Reboot' 2>/tmp/ans
     ANS=`cat /tmp/ans`
     case $ANS in
        vesa) cp /etc/scripts/cardDetect/XF86Config.compat /etc/X11/xorg.conf 
              echo "Copied failsafe 1024x768 VESA config."
              echo "[Press Enter to Continue]"
              read tmp
              ;;
        auto) X -configure
              cat /root/xorg.conf.new >> /etc/X11/xorg.conf
              clear
              echo "Copied Xorg auto-detected config."
              echo "[Press Enter to Continue]"
              read tmp
              ;;
        viconfig) vi /etc/X11/xorg.conf
                    echo "[Press Enter to Continue]"
                    read tmp
                    ;;
        shell) /bin/tcsh ;;
        save) echo "Preparing to upload /etc/X11/xorg.conf to the server."
    	      echo "You will need to enter the password for user 'pxeboot' in order to proceed."		 
              echo "[Press Enter to Continue]"
              read tmp
              cp /etc/X11/xorg.conf /tmp/${MAC}.conf
              echo "put /tmp/${MAC}.conf" > /tmp/uploadcmd
              sftp pxeboot@192.168.2.2 </tmp/uploadcmd
              rm /tmp/uploadcmd >/dev/null 2>/dev/null
              echo "[Press Enter to Continue]"
              read tmp
              ;;
        test) echo "#!/bin/sh
echo 'Accept this configuration? (Y/N)'
read -t15s response 
if [ \"${response}\" = \"Y\" -o \"${response}\" = \"y\" ]
then
  touch /tmp/workingX
fi" >/tmp/testX
	      chmod 755 /tmp/testX
              rm /tmp/workingX >/dev/null 2>/dev/null 

              echo "#!/bin/sh
xv -root -quit -max /etc/scripts/wallpaper.png
xterm -e /bin/sh -c /tmp/testX" > /root/.xinitrc
              chmod 755 /root/.xinitrc
                
              echo "Preparing to test X. If the screen does not display, wait 15 seconds and X will exit."
              echo "[Press Enter to Continue]"
              read tmp
              startx 
              if [ -e "/tmp/testX" ]
              then
                echo "Congratulations! X has been correctly configured."
                echo "Please select 'save working config' to upload the configuration to the server. "
              fi
              echo "[Press Enter to Continue]"
              read tmp
               ;;
        close) reboot ;;
        *) ;;
     esac
     done

  else
    # User didn't want to config X, run with autodetect
    # Now bring up the X session to query for XDM servers
    echo "Auto-detecting X settings"
    /usr/local/bin/X -configure
    cp /root/xorg.conf.new /etc/X11/xorg.conf
    /usr/local/bin/X -broadcast
    reboot
  fi

fi
