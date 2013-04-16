#!/bin/sh
DE_NAME="LXDE"
XDG="YES"
DE_FILEMAN="pcmanfm %s"
DE_TERMINAL="lxterminal"
DE_TERMINAL_TITLE="-T"
DE_TERMINAL_COMMAND="-e"

active()
{
if [ "$DESKTOP_SESSION" = "LXDE" ] ; then
DE_ACTIVE=1
fi
}

installed()
{
# Check via pc-metapkgmanager if it exists
which pc-metapkgmanager >/dev/null 2>/dev/null
if [ $? -eq 0 ] ; then
  pc-metapkgmanager status LXDE | grep installed >/dev/null
  if [ $? -eq 0 ] ; then
    DE_INSTALLED=1
  fi
else
  which startlxde >/dev/null 2>/dev/null
  if [ $? -eq 0 ] ; then
    DE_INSTALLED=1
  fi
fi
}