#!/bin/sh
DE_NAME="XFCE"
XDG="YES"
DE_SU="pc-su %s"
DE_FILEMAN="thunar %s"
DE_TERMINAL="xfce4-terminal"
DE_TERMINAL_TITLE="-T"
DE_TERMINAL_COMMAND="-e"
DE_SESSION_NAME="XFCE"

active() {
if [ "$PCDM_SESSION" = "$DE_SESSION_NAME" ]; then
DE_ACTIVE=1
fi  

}

installed()
{
# Check via pc-metapkgmanager if it exists
which pc-metapkgmanager >/dev/null 2>/dev/null
if [ $? -eq 0 ] ; then
  pc-metapkgmanager status XFCE | grep "is installed">/dev/null
  if [ $? -eq 0 ] ; then
    DE_INSTALLED=1
  fi
else
  which startxfce4 >/dev/null 2>/dev/null
  if [ $? -eq 0 ] ; then
    DE_INSTALLED=1
  fi
  which startxfce >/dev/null 2>/dev/null
  if [ $? -eq 0 ] ; then
    DE_INSTALLED=1
  fi
fi
}