DE_NAME="Gnome"
XDG="YES"
DE_SU="gksu %s"
DE_FILEMAN="nautilus %s"
DE_TERMINAL="gnome-terminal"
DE_TERMINAL_TITLE="-t"
DE_TERMINAL_COMMAND="-e"

active() {
if [ "$DESKTOP_SESSION" = "gnome" ] || [ "$DESKTOP_SESSION" = "02gnome" ] || [ "$DESKTOP_SESSION" = "openbox-gnome"  ] ; then
DE_ACTIVE=1
fi  

}

installed() {
# Check via pc-metapkgmanager if it exists
which pc-metapkgmanager >/dev/null 2>/dev/null
if [ $? -eq 0 ] ; then
  pc-metapkgmanager status GNOME | grep "is installed" >/dev/null
  if [ $? -eq 0 ] ; then
    DE_INSTALLED=1
  fi
else
  which gnome-session >/dev/null 2>/dev/null
  if [ $? -eq 0 ] ; then
    DE_INSTALLED=1
  fi
fi

}
