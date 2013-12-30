DE_NAME="Cinnamon"
XDG="YES"
DE_SU="pc-su %s"
DE_FILEMAN="nautilus %s"
DE_TERMINAL="gnome-terminal"
DE_TERMINAL_TITLE="-t"
DE_TERMINAL_COMMAND="-e"

active() {
if [ "$DESKTOP_SESSION" = "cinnamon" ] || [ "$DESKTOP_SESSION" = "32Cinnamon" ] || [ "$DESKTOP_SESSION" = "openbox-cinnamon"  ] ; then
DE_ACTIVE=1
fi  

}

installed() {
# Check via pc-metapkgmanager if it exists
which pc-metapkgmanager >/dev/null 2>/dev/null
if [ $? -eq 0 ] ; then
  pc-metapkgmanager status Cinnamon | grep "is installed" >/dev/null
  if [ $? -eq 0 ] ; then
    DE_INSTALLED=1
  fi
else
  which gnome-session-cinnamon >/dev/null 2>/dev/null
  if [ $? -eq 0 ] ; then
    DE_INSTALLED=1
  fi
fi

}
