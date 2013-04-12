#!/bin/sh

check_pid()
{
  PID="$1"
  ps -p "${PID} " >/dev/null 2>/dev/null
  if [ "$?" = "0" ]
  then
    return 1
  else
    return 0
  fi
};

get_setting()
{
  SETTINGS="$1"
  KEY="$2"
  VAL=""

  if [ -e "${SETTINGS}" ]
  then
    grep "^${KEY}: " ${SETTINGS} >/dev/null 2>/dev/null
    if [ "$?" = "0" ]
    then
      VAL=`grep "^${KEY}: " ${SETTINGS} | sed "s|${KEY}: ||g"` 
    fi
  fi

  export VAL
};
