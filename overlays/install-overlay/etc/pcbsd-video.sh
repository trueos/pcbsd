#!/bin/sh
# Detect if we need to load nvidia, or skip it due to optimus setup
###################################################################
PATH="${PATH}:/usr/bin:/usr/sbin"
export PATH

card1=""
card2=""
curCard=0
inCard=0

pciconf -lv > /tmp/.pciconf.$$
while read line
do
   echo $line | grep -q "^vgapci" 
   if [ $? -eq 0 ] ; then curCard=`expr $curCard + 1` ; inCard=1; continue; fi
   
   echo $line | grep -q "subclass" 
   if [ $? -eq 0 ] ; then inCard=0; continue;  fi

   if [ $inCard -eq 1 ] ; then
      echo $line | grep -q "vendor"
      if [ $? -eq 0 ]; then
	 case $curCard in
	    1) card1=`echo $line | cut -d "'" -f 2` ;;
	    2) card2=`echo $line | cut -d "'" -f 2` ;;
	    *) ;;
	 esac
      fi

   fi
done < /tmp/.pciconf.$$
rm /tmp/.pciconf.$$

if [ -n "$card1" ] ; then
  echo "VideoCard1: $card1"
  sleep 1
fi
if [ -n "$card2" ] ; then
  echo "VideoCard2: $card2"
  sleep 1
fi

loadIntelKMS=no

# Check for optimus with intel as first card
echo "$card1" | grep -q -i -e "intel"
if [ $? -eq 0 ] ; then
   echo "$card2" | grep -q -i -e "nvidia"
   if [ $? -eq 0 ] ; then
      echo "Detected Optimus! Using intel video..."
      sleep 1
   fi
   loadIntelKMS=yes
fi

# Check for optimus with nvidia as first card
echo "$card1" | grep -q -i -e "nvidia"
if [ $? -eq 0 ] ; then
   echo "$card2" | grep -q -i -e "intel"
   if [ $? -eq 0 ] ; then
      echo "Detected Optimus! Using intel video..."
      sleep 1
      loadIntelKMS=yes
   else
      echo "Detected NVIDIA! Loading NVIDIA driver..."
      kldload nvidia
      sleep 1
   fi
fi
