#!/bin/sh

stop_powerd()
{
   if [ -e "/var/run/powerd.pid" ] ; then
      kill -9 "`cat /var/run/powerd.pid`"
   fi
}

powerd_up()
{
  stop_powerd
  powerd -P /var/run/powerd.pid -a hiadaptive -b adaptive 2>/dev/null
}

powerd_down()
{
  stop_powerd
  powerd -P /var/run/powerd.pid -a adaptive -b min 2>/dev/null
}

# Type of event open/close
EVENT=$1

case $EVENT in
  close) powerd_down ;;
      *) powerd_up ;;
esac
