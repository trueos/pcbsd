#!/bin/sh

# If you want to unconditionally enable ibus modify ~/.xprofile
# by adding the line:  FORCEIBUS=YES

# default to NO if not set in ~/.xprofile
: ${FORCEIBUS:="NO"}

# Do not modify below
##########################################################################

ibus_initialize() {
  if [ ! -f ${HOME}/.gconf/desktop/ibus/general/%gconf.xml ]
  then
    gconftool-2 --set --type list --list-type=string /desktop/ibus/general/preload_engines "[${1}]"
  fi
}

# Check if this lang needs ibus
case ${LANG} in
  ja_JP*)
  ibus_initialize "mozc-jp"
  ENABLE="YES"
  ;;
  ko_KR*)
  ibus_initialize "m17n:ko:romaja"
  ENABLE="YES"
  ;;
  zh_CN*)
  ibus_initialize "pinyin"
  ENABLE="YES"
  ;;
  zh_TW*)
  ibus_initialize "chewing"
  ENABLE="YES"
  ;;
  *) ENABLE="NO" ;;
esac

# If the user requested to enable IBUS manually
if [ "${FORCEIBUS}" = "YES" ]
then
  ENABLE="YES"
fi

# If we are using ibus, set vars and enable daemon
if [ "${ENABLE}" = "YES" ]
then
  # In some PBI, iBus GTK IM module makes GTK version mismatch error
  # with GTK libraries included in the PBI package.
  #GTK_IM_MODULE="ibus" ; export GTK_IM_MODULE
 
  QT_IM_MODULE="ibus" ; export QT_IM_MODULE
  XMODIFIERS="@im=ibus" ; export XMODIFIERS

  # For PBI applications
  GTK_IM_MODULE="xim" ; export GTK_IM_MODULE
  GTK_IM_MODULE_FILE=/usr/local/etc/gtk-2.0/gtk.immodules ; export GTK_IM_MODULE_FILE
  /usr/local/bin/ibus-daemon --xim &

  # Enable GDK_NATIVE_WINDOWS
  GDK_NATIVE_WINDOWS="true" ; export GDK_NATIVE_WINDOWS
fi
