#!/bin/sh


cd /home
for i in `ls` 
   do 
     if [ -d "/home/${i}/.xlog" ] 
     then 
       if [ "$?" = "0" ] 
       then 
         rm -R /home/${i}/.xlog
       fi 
     fi 
   done
