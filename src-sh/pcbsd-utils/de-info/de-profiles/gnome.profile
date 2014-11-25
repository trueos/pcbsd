#############################################################################################
#  This is Desktop environment profile for de-info script
# To add new desktop environment you should define at least those variables:
#
# $DE_NAME - desktop environment name to display
# 
#      --Detection of desktop environment installation--
#  You may define one or all of those variables:
# $DE_METAPACKAGE - name of desktop environment meta package
# $DE_INSTALL_FILE - list of executable files related to desktop environment separated by space
#    -or-
# if $DE_METAPACKAGE and $DE_INSTALL_FILE are undefined or empty you should implement installed()
# function that set DE_INSTALLED to 1 if desktop environment is installed
# For example:
#
# installed() {
#   pc-metapkgmanager status Cinnamon | grep "is installed" >/dev/null
#   if [ $? -eq 0 ] ; then
#     DE_INSTALLED=1
#   fi
# }
#
#      --Current desktop environment detection--
# PCDM writes to $PCDM_SESSION environment variable name of current desktop environment.
# You may define this for checking is defined desktop environment current:
# $DE_SESSION_NAME - name of desktop environment PCDM session
#    -or-
# If $DE_SESSION_NAME undefined or empty you should implement current() function that
# should set DE_ACTIVE to 1 if desktop environment is active now
# For example:
#
# active() {
# if [ "$PCDM_SESSION" = "$DE_SESSION_NAME" ]; then
#   DE_ACTIVE=1
# fi  
# }
# 
#      --Another fields---
# $XDG - set to "YES" if desktop environment is fully compatible with xdg-open script
# $DE_CONFIG_APP - name of desktop environment main configuration application (if present)
# Other fields are deprocated
#
#      --Notes---
# DE profile is an processes as shell script, so you may use shell scripting techniques.
# For example:
# DE_CONFIG_APP=de_configapp --user `whoami`
#############################################################################################

DE_NAME="Gnome"
XDG="NO"

DE_SESSION_NAME="GNOME"
DE_METAPACKAGE="KDE"
DE_INSTALL_FILE="gnome-session"

DE_CONFIG_APP="gnome-control-center --overview"

DE_LOGOFF_COMMAND="gnome-session-quit"

DE_SU="gksu %s"
DE_FILEMAN="nautilus %s"
DE_TERMINAL="gnome-terminal"
DE_TERMINAL_TITLE="-t"
DE_TERMINAL_COMMAND="-e"

