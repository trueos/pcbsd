DE_NAME="KDE"
XDG="YES"

DE_SU="kdesu %s"
DE_TERMINAL="konsole"
DE_TERMINAL_TITLE="--caption"
DE_TERMINAL_COMMAND="-e"
DE_TERMINAL_SEPARATE_ARGS="YES"
DE_FILEMAN="dolphin %s"
DE_SESSION_NAME="KDE"

active() {

 if [ -n "$KDE_SESSION_UID" ] || [ "${XDG_CURRENT_DESKTOP}" == "KDE" ] || [ "$DE_SESSION_NAME" == "$PCDM_SESSION" ] ; then
    DE_ACTIVE=1
 fi

}


installed()
{
# Check via pc-metapkgmanager if it exists
which pc-metapkgmanager >/dev/null 2>/dev/null
if [ $? -eq 0 ] ; then
  pc-metapkgmanager status KDE | grep "is installed" >/dev/null
  if [ $? -eq 0 ] ; then
    DE_INSTALLED=1
  fi
else
  which startkde4 >/dev/null 2>/dev/null
  if [ $? -eq 0 ] ; then
    DE_INSTALLED=1
  fi
  which startkde >/dev/null 2>/dev/null
  if [ $? -eq 0 ] ; then
    DE_INSTALLED=1
  fi
fi

}
